#include "RVCOS.h"
#include "esc_codes.h"
#include "queues.h"

volatile char* VIDEO_MEMORY = (volatile char*)(0x50000000 + 0xFE800);
volatile uint32_t* main_gp = 0;

TCB** global_tcb_arr = NULL;
MemoryPoolController* global_mem_pool_arr = NULL;  // TODO change the 10
MUTEX** global_mutex_arr;

uint32_t curr_available_mem_pool_index = 0;
uint32_t curr_TCB_max_size = 256;

PriorityQueue* low_prio;
PriorityQueue* med_prio;
PriorityQueue* high_prio;
PriorityQueue* idle_prio;
PriorityQueue* wait_q;

volatile uint32_t last_write_pos = 0;
volatile uint32_t running_thread_id = 1;

uint32_t call_on_other_gp(void* param, TEntry entry, uint32_t* gp);
void ContextSwitch(volatile uint32_t **oldsp, volatile uint32_t *newsp); // ? is the old sp ** or *
uint32_t* initStack(uint32_t *sp, TEntry function, uint32_t param, uint32_t tp);

void threadSkeleton(uint32_t thread) {
  asm volatile("csrsi mstatus, 0x8");  // enable interrupts

  // call entry(param) but make sure to switch the gp right before the call
  global_tcb_arr[running_thread_id]->state = RVCOS_THREAD_STATE_RUNNING;
  uint32_t ret_val = call_on_other_gp(global_tcb_arr[thread]->param, global_tcb_arr[thread]->entry, main_gp);
  RVCThreadTerminate(thread, ret_val);
}

/**
 * @brief Thread Scheduler, only call this through timer interrupt OR thread terminate
 * 
 */
void schedule(uint32_t test_new_id) {
  uint32_t old_id = running_thread_id;
  uint32_t new_id = 0;
  if (high_prio->size > 0) {
    dequeue(high_prio, &new_id);
    running_thread_id = new_id;
  }
  else if (med_prio->size > 0) {
    dequeue(med_prio, &new_id);
    running_thread_id = new_id;
  }
  else if (low_prio->size > 0) {
    dequeue(low_prio, &new_id);
    running_thread_id = new_id;
  }
  else {
    running_thread_id = 0;
  }
  // ContextSwitch(global_tcb_arr[old_id]->sp, global_tcb_arr[new_id]->sp);
  WriteString("try to switch\n");

  // !Problem: cannot write the new sp value into SP register, it jumps straight to interrupt handler mcause 0x0..007
  ContextSwitch(global_tcb_arr[old_id]->sp, global_tcb_arr[test_new_id]->sp);
  WriteString("back\n");
}

/**
 * @brief Writes a string to the screen
 *
 * @param str string to write
 */
void WriteString(const char* str) {
  const char* Ptr = str;
  while (*Ptr) {
    Ptr++;
  } 
  RVCWriteText(str, Ptr - str);
}

void writeInt(uint32_t val) {
  char* val_text[33];
  itoa(val, val_text, 10);
  WriteString(val_text);
}

void idleFunction() {
  asm volatile("csrci mstatus, 0x8");  // enables interrupts
  while (1) {
    ;
  }
}

uint32_t getNextAvailableTCBIndex() {
  for (uint32_t i = 0; i < curr_TCB_max_size; i++) {
    if (!global_tcb_arr[i]) {  // if the curr slot is empty
      return i;
    }
  }
  return -1;  // no available slots
}

uint32_t getNextAvailableMemPoolIndex() {
  curr_available_mem_pool_index++;
  return curr_available_mem_pool_index - 1;
}

uint32_t getNextAvailableMUTEXIndex() {
  for (uint32_t i = 0; i < 1024; i++)  // Arbitrary 1024
  {
    if (!global_mutex_arr[i]) {
      return i;
    }
  }
  return -1;
}

PriorityQueue* getPQByPrioNum(uint32_t prio_num) {
  switch (prio_num) {
  case 3:
    return high_prio;
    break;
  case 2:
    return med_prio;
    break;
  case 1:
    return low_prio;
    break;
  case 0:
    return idle_prio;
  }
}

void resizeTCBArray() {
  TCB** new_tcb_arr = malloc(sizeof(TCB*) * 2 * curr_TCB_max_size);
  for (int i = 0; i < curr_TCB_max_size; i++) {
    new_tcb_arr[i] = global_tcb_arr[i];
  }
  curr_TCB_max_size *= 2;
  free(global_tcb_arr);
  global_tcb_arr = new_tcb_arr;
}

TStatus RVCInitialize(uint32_t* gp) {
  if (!gp) {
    return RVCOS_STATUS_ERROR_INVALID_STATE;
  }
  main_gp = gp;

  global_tcb_arr = malloc(sizeof(TCB*) * 256);  // TODO: remove 256 cap
  for (uint32_t i = 0; i < 256; i++) {
    global_tcb_arr[i] = NULL;
  }  // manual calloc

  low_prio = (PriorityQueue*)malloc(sizeof(PriorityQueue));
  med_prio = (PriorityQueue*)malloc(sizeof(PriorityQueue));
  high_prio = (PriorityQueue*)malloc(sizeof(PriorityQueue));
  idle_prio = (PriorityQueue*)malloc(sizeof(PriorityQueue));

  low_prio->head = med_prio->head = high_prio->head = idle_prio->head = NULL;
  low_prio->tail = med_prio->tail = high_prio->tail = idle_prio->tail = NULL;
  low_prio->size = med_prio->size = high_prio->size = idle_prio->size = 0;

  uint32_t* idle_tid = malloc(sizeof(uint32_t));
  RVCThreadCreate(idleFunction, NULL, 1024, RVCOS_THREAD_PRIORITY_IDLE, idle_tid);
  free(idle_tid);

  // Creating MAIN thread and MAIN thread TCB manually because it's a special case
  TCB* main_thread_tcb = malloc(sizeof(TCB));
  main_thread_tcb->thread_id = MAIN_THREAD_ID;

  main_thread_tcb->state = RVCOS_THREAD_STATE_RUNNING;
  main_thread_tcb->sp = 0x71000000;      // top of physical stack
  main_thread_tcb->mem_size = 0xE00000;  //? is this 14MB
  main_thread_tcb->priority = 2;

  global_tcb_arr[1] = main_thread_tcb;

  return RVCOS_STATUS_SUCCESS;
}

TStatus RVCThreadDelete(TThreadID thread) {
  if (!global_tcb_arr[thread]) {
    return RVCOS_STATUS_ERROR_INVALID_ID;
  }

  if (global_tcb_arr[thread]->state != RVCOS_THREAD_STATE_DEAD) {
    return RVCOS_STATUS_ERROR_INVALID_STATE;
  }

  uint32_t tid;  // here just for the function param, not used
  dequeue(getPQByPrioNum(global_tcb_arr[thread]->priority), &tid);
  free(global_tcb_arr[thread]->sp);
  free(global_tcb_arr[thread]);

  return RVCOS_STATUS_SUCCESS;
}

/**
 * @brief
 *
 * @param entry function pointer to the functon we want to run
 * @param param param array
 * @param memsize memsize for the entire thread
 * @param prio priority
 * @param tid thread id pointer from main to save out tid
 * @return TStatus
 */

TStatus RVCThreadCreate(TThreadEntry entry, void* param, TMemorySize memsize,
  TThreadPriority prio, TThreadIDRef tid) {
  if (!entry || !tid) {
    return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
  }

  TCB* curr_thread_tcb = malloc(sizeof(TCB));
  void* thread_memory = malloc(memsize);

  curr_thread_tcb->priority = prio;
  curr_thread_tcb->state = RVCOS_THREAD_STATE_CREATED;
  curr_thread_tcb->mem_size = memsize;
  curr_thread_tcb->entry = entry;
  curr_thread_tcb->param = param;

  *tid = getNextAvailableTCBIndex();
  if (*tid == -1) {
    resizeTCBArray();
    *tid = getNextAvailableTCBIndex();
  }
  else {
    curr_thread_tcb->thread_id = *tid;
    global_tcb_arr[*tid] = curr_thread_tcb;
  }

  writeInt(prio);
  WriteString(" to enq\n");
  enqueue(getPQByPrioNum(prio), *tid);
  return RVCOS_STATUS_SUCCESS;
}

TStatus RVCThreadActivate(TThreadID thread) {
  /**
   * Init stack here
   * malloc for each threads stack base @789 on piazza
   */

  if (global_tcb_arr[thread] == NULL) {
    WriteString("bad thread");
    return RVCOS_STATUS_ERROR_INVALID_ID;
  }
  uint32_t state = global_tcb_arr[thread]->state;
  if (!(state == RVCOS_THREAD_STATE_CREATED || state == RVCOS_THREAD_STATE_DEAD)) {
    writeInt(state);
    WriteString(" is bad state\n");
    return RVCOS_STATUS_ERROR_INVALID_STATE;
  }

  global_tcb_arr[thread]->sp = malloc(global_tcb_arr[thread]->mem_size);
  global_tcb_arr[thread]->state = RVCOS_THREAD_STATE_READY;

  global_tcb_arr[thread]->sp = initStack(global_tcb_arr[thread]->sp, global_tcb_arr[thread]->entry, global_tcb_arr[thread]->param, thread);

  uint32_t tid;
  dequeue(getPQByPrioNum(global_tcb_arr[thread]->priority), &tid);
  //  set thread to STATUS_RUNNING
  
  writeInt(running_thread_id);
  WriteString(" is old thread\n");
  
  writeInt(global_tcb_arr[running_thread_id]->sp);
  WriteString(" is old sp\n");

  writeInt(tid);
  WriteString(" is the id from deq\n");
  // TODO call the skeleton function here

  schedule(global_tcb_arr[thread]->thread_id); 
  threadSkeleton(running_thread_id);

  return RVCOS_STATUS_SUCCESS;
}

TStatus RVCThreadTerminate(TThreadID thread, TThreadReturn returnval) {
  if (!global_tcb_arr[thread]) {
    return RVCOS_STATUS_ERROR_INVALID_ID;
  }

  uint32_t state = global_tcb_arr[thread]->state;
  if (state == RVCOS_THREAD_STATE_CREATED || state == RVCOS_THREAD_STATE_DEAD) {
    return RVCOS_STATUS_ERROR_INVALID_STATE;
  }

  global_tcb_arr[thread]->state = RVCOS_THREAD_STATE_DEAD;
  returnval = global_tcb_arr[thread]->ret_val;

  // call scheduler here 
  return RVCOS_STATUS_SUCCESS;
}

/**
 * @brief Write the contents of buffer to the screen
 * this writes to a new line everytime the function is called
 *
 * @param buffer
 * @param writesize
 * @return TStatus
 */
TStatus RVCWriteText(const TTextCharacter* buffer, TMemorySize writesize) {
  uint32_t n_pos = 0;
  uint32_t physical_write_pos = last_write_pos;

  // For escape codes
  uint32_t row_int;
  uint32_t col_int;

  for (uint32_t j = 0; j < writesize; j++) {
    if ((uint32_t)buffer[j + 2] == '\x1B' && (uint32_t)buffer[j + 3] == '[') {
      if (writesize > 7) {
        char row[3];
        char col[3];

        // These variables will probably need to be declared in a broader
        // scope when integrated
        uint32_t row_int;
        uint32_t col_int;

        if ((uint32_t)buffer[j + 5] == ';') {  // Row is single digit
          strncpy(row, (buffer + j) + 4, 1);
          row[1] = '\0';
          row_int = atoi(row);

          if ((uint32_t)buffer[j + 7] == 'H') {  // Col is single digit
            strncpy(col, (buffer + j) + 6, 1);
            col[1] = '\0';
            col_int = atoi(col);
          }
          else if ((uint32_t)buffer[j + 8] == 'H') {  // Col is double digit
            strncpy(col, (buffer + j) + 6, 2);
            col[2] = '\0';
            col_int = atoi(col);
            ;
          }
          else {
            break;
          }
        }
        else if ((uint32_t)buffer[j + 6] == ';') {  // Row is double digit
          strncpy(row, (buffer + j) + 4, 2);
          row[2] = '\0';
          row_int = atoi(row);

          if ((uint32_t)buffer[j + 8] == 'H') {  // Col is single digit
            strncpy(col, (buffer + j) + 7, 1);
            col[1] = '\0';
            col_int = atoi(col);
          }
          else if ((uint32_t)buffer[j + 9] == 'H') {  // Col is double digit
            strncpy(col, (buffer + j) + 7, 2);
            col[2] = '\0';
            col_int = atoi(col);
          }
          else {
            break;
          }
        }

        // Final check to ensure integer values are within range
        uint32_t row_check = (row_int >= 0 && row_int < 36) ? 1 : 0;
        uint32_t col_check = (col_int >= 0 && col_int < 64) ? 1 : 0;

        if (!row_check || !col_check) {
          WriteString("Invalid Parameters");
        }
        else {
          WriteString("Moving cursor to specified row and column");
          // Code to move cursor to row and column
          // BE CAREFUL of 'off by one' errors here
          // It's assumed escape code will use 0-35 for rows and 0-63
          // for columns
        }
      }

      // Sent to external esc_codes switch
      else {
        uint32_t code = esc_codes(buffer, writesize);

        switch (code) {
        case 1: {
          RVCWriteText("\b ", 2);
          last_write_pos = physical_write_pos - 65 > 0
            ? physical_write_pos - 65
            : physical_write_pos;
          RVCWriteText("X", 1);
          // code to move cursor up
          break;
        }
        case 2: {
          RVCWriteText("\b ", 2);
          if (last_write_pos == 0) {
            VIDEO_MEMORY[0] = ' ';
            last_write_pos = 64;
          }
          else {
            last_write_pos = (physical_write_pos + 63) % MAX_VRAM_INDEX;
          }
          RVCWriteText("X", 1);
          // code to move cursor down
          break;
        }
        case 3: {
          RVCWriteText("\b ", 2);
          RVCWriteText("X", 1);
          // code to move cursor right
          break;
        }
        case 4: {
          RVCWriteText("\b \b\b", 4);
          RVCWriteText("X", 1);
          // code to move cursor left
          break;
        }
        case 5: {
          // code to move cursor upper left
          break;
        }
        case 6: {
          WriteString("2: Erase screen, leave cursor");
          for (int h = 0; h < MAX_VRAM_INDEX; h++) {
            VIDEO_MEMORY[h] = '\0';
          }
          RVCWriteText("X", 1);
          break;
        }
        default:
          break;
        }
      }
      return RVCOS_STATUS_SUCCESS;
    }
    if (buffer[j] == '\n') {
      uint32_t next_line = (physical_write_pos / 64) + 1;
      physical_write_pos = next_line * 64;
      physical_write_pos -= j;  // now j is not 0 anymore, so push
                                // physical_write_pos back by j
      n_pos = j - 1;
    }
    else if (buffer[j] == '\b') {
      physical_write_pos = physical_write_pos - 2 > 0
        ? physical_write_pos - 2
        : 0;  // make sure writepos is always >=0
    }
    VIDEO_MEMORY[physical_write_pos++] = buffer[j];
  }

  // change this line to change the behavior of writing to a filled screen/
  // now it just goes back to 0 and overwrites what's on screen
  last_write_pos = (physical_write_pos + n_pos) % MAX_VRAM_INDEX;

  return RVCOS_STATUS_SUCCESS;
}

TStatus RVCThreadID(TThreadIDRef threaddref) {
  if (!threaddref) {
    return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
  }
  *threaddref = running_thread_id;
  return RVCOS_STATUS_SUCCESS;
}

TStatus RVCThreadState(TThreadID thread, TThreadStateRef state) {
  if (!global_tcb_arr[thread]) {
    return RVCOS_STATUS_ERROR_INVALID_ID;
  }
  if (!state) {
    return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
  }
  *state = global_tcb_arr[thread]->state;
  return RVCOS_STATUS_SUCCESS;
}

TStatus RVCThreadWait(TThreadID thread, TThreadReturnRef returnref,
  TTick timeout) {
  if (!global_tcb_arr[thread]) {
    return RVCOS_STATUS_ERROR_INVALID_ID;
  }
  if (returnref == NULL) {
    return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
  }

  return RVCOS_STATUS_SUCCESS;
}

TStatus RVCThreadSleep(TTick tick) { return RVCOS_STATUS_SUCCESS; }

TStatus RVCTickMS(uint32_t* tickmsref) {
  if (tickmsref) {
    uint32_t time = TIME_REG * 1000;
    *tickmsref = time;
    return RVCOS_STATUS_SUCCESS;
  }
  else {
    return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
  }

  return RVCOS_STATUS_SUCCESS;
}

//? what does this function do?
TStatus RVCTickCount(TTickRef tickref) {
  if (tickref) {
    uint32_t time = TIME_REG;
    *tickref = time;
    return RVCOS_STATUS_SUCCESS;
  }
  else {
    return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
  }
}

TStatus RVCReadController(SControllerStatusRef statusref) {
  if (!statusref) {
    return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
  }

  statusref->DLeft = CONTROLLER_REG_VAL >> 0 & 0x1;
  statusref->DUp = CONTROLLER_REG_VAL >> 1 & 0x1;
  statusref->DDown = CONTROLLER_REG_VAL >> 2 & 0x1;
  statusref->DRight = CONTROLLER_REG_VAL >> 3 & 0x1;

  statusref->DButton1 = CONTROLLER_REG_VAL >> 4 & 0x1;
  statusref->DButton2 = CONTROLLER_REG_VAL >> 5 & 0x1;
  statusref->DButton3 = CONTROLLER_REG_VAL >> 6 & 0x1;
  statusref->DButton4 = CONTROLLER_REG_VAL >> 7 & 0x1;

  statusref->DReserved = CONTROLLER_REG_VAL >> 8;

  return RVCOS_STATUS_SUCCESS;
}

TStatus RVCMemoryPoolCreate(void* base, TMemorySize size,
  TMemoryPoolIDRef memoryref) {
  if (base == NULL || size < 2 * MIN_ALLOC_SIZE || memoryref == NULL) {
    return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
  }
  *memoryref = getNextAvailableMemPoolIndex();

  MemoryPoolController pool;

  // base->||controller||chunk|data||
  pool.first_chunk = base + sizeof(MemoryPoolController);  
  pool.pool_id = *memoryref;
  pool.pool_size = size;
  pool.bytes_left = size;

  pool.first_chunk->isFree = 1;
  pool.first_chunk->data_size = size;
  pool.first_chunk->next = NULL;
  pool.first_chunk->prev = NULL;

  global_mem_pool_arr[*memoryref] = pool;

  return RVCOS_STATUS_SUCCESS;
}

TStatus RVCMemoryPoolDelete(TMemoryPoolID memory) {
  if (memory < 0) {
    return RVCOS_STATUS_ERROR_INVALID_ID;
  }
  if (global_mem_pool_arr[memory].first_chunk == NULL) {
    free(global_mem_pool_arr[memory].first_chunk -
      sizeof(MemoryPoolController));
    return RVCOS_STATUS_SUCCESS;
  }
  else {
    return RVCOS_STATUS_ERROR_INVALID_STATE;
  }
}

TStatus RVCMemoryPoolQuery(TMemoryPoolID memory, TMemorySizeRef bytesleft) {
  if (memory < 0) {
    return RVCOS_STATUS_ERROR_INVALID_ID;
  }
  *bytesleft = global_mem_pool_arr[memory].bytes_left;
  return RVCOS_STATUS_SUCCESS;
}

/**
 * @brief Allocates space from a given Memory Pool
 *
 * @param memory a already initialized mem pool
 * @param size num bytes to alloc in this memory pool
 * @param pointer pointer to the allocated space
 * @return TStatus
 *
 * @Notes
 * This assumes memory is a already initialized mem pool except when
 * memory == 0
 */
TStatus RVCMemoryPoolAllocate(TMemoryPoolID memory, TMemorySize size,
  void** pointer) {
  if (memory < 0) {
    return RVCOS_STATUS_ERROR_INVALID_ID;
  }
  if (!size || !pointer) {
    return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
  }

  if (memory == RVCOS_MEMORY_POOL_ID_SYSTEM && global_mem_pool_arr == NULL) {
    uint32_t max_chunk_count = (size / MIN_ALLOC_SIZE) > 2 ? (size / MIN_ALLOC_SIZE) : 2;
    *pointer = malloc(size + sizeof(MemoryPoolController) + sizeof(MemoryChunk) * max_chunk_count);
    global_mem_pool_arr = malloc(10 * sizeof(MemoryPoolController*));

    return RVCOS_STATUS_SUCCESS;
  }

  // do some normal mem pool stuff
  MemoryPoolController pool = global_mem_pool_arr[memory];
  if (!&pool) {
    return RVCOS_STATUS_ERROR_INVALID_ID;
  }
  TMemorySize actual_alloc_size = ((size % 64) + 1) * 64;

  MemoryChunk* curr_chunk = pool.first_chunk;
  while (curr_chunk) {
    if (curr_chunk->isFree && curr_chunk->data_size >= actual_alloc_size) {
      // mark current as an occupied chunk
      curr_chunk->isFree = 0;
      *pointer = curr_chunk + sizeof(MemoryChunk);  // move offset to actual mem location

      // now make the free chunk
      // ! chueck if it's in bounds
      MemoryChunk* free_chunk = curr_chunk + sizeof(MemoryChunk) + actual_alloc_size;

      free_chunk->isFree = 1;
      free_chunk->prev = curr_chunk;
      free_chunk->next = curr_chunk->next;

      // TODO: check if this expression is correct
      free_chunk->data_size = curr_chunk->data_size - sizeof(MemoryChunk) - actual_alloc_size;  
      

      curr_chunk->next->prev = free_chunk;
      pool.bytes_left -= sizeof(MemoryChunk) + actual_alloc_size;

      break;
    }
    curr_chunk = curr_chunk->next;
  }

  return RVCOS_STATUS_SUCCESS;
}

/**
 * @brief Releases the chunk pointed by pointer back to memory pool
 *
 * @param memory the pool id receiving the released chunk
 * @param pointer pointer to the chunk that needs to be released
 * @return TStatus
 */
TStatus RVCMemoryPoolDeallocate(TMemoryPoolID memory, void* pointer) {
  if (memory < 0) {
    return RVCOS_STATUS_ERROR_INVALID_ID;
  }
  if (!pointer) {
    return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
  }

  MemoryPoolController pool = global_mem_pool_arr[memory];
  MemoryChunk* chunk_to_return = pointer - sizeof(MemoryChunk);

  if (chunk_to_return->prev == NULL) {


  }
  else {
    chunk_to_return->prev->data_size +=
      chunk_to_return->data_size + sizeof(MemoryChunk);
    chunk_to_return->prev->next = chunk_to_return->next;
    chunk_to_return->next->prev = chunk_to_return->prev;
  }

  return RVCOS_STATUS_SUCCESS;
}

TStatus RVCMutexCreate(TMutexIDRef mutexref) {
  if (!mutexref) {
    return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
  }

  // Need to check this and return failure if it fails
  MUTEX* curr_mutex = malloc(sizeof(MUTEX));
  if (!curr_mutex) {
    return RVCOS_STATUS_ERROR_INSUFFICIENT_RESOURCES;
  }

  curr_mutex->state = RVCOS_MUTEX_STATE_UNLOCKED;
  *mutexref = getNextAvailableMUTEXIndex();
  curr_mutex->mutex_id = *mutexref;
  global_mutex_arr[*mutexref] = curr_mutex;
  return RVCOS_STATUS_SUCCESS;
}

TStatus RVCMutexDelete(TMutexID mutex) {
  if (!global_mutex_arr[mutex]) {
    return RVCOS_STATUS_ERROR_INVALID_ID;
  }
  if (global_mutex_arr[mutex]->owner) {
    return RVCOS_STATUS_ERROR_INVALID_STATE;
  }

  free(global_mutex_arr[mutex]);
  return RVCOS_STATUS_SUCCESS;
}

TStatus RVCMutexQuery(TMutexID mutex, TThreadIDRef ownerref) {
  if (!global_mutex_arr[mutex]) {
    return RVCOS_STATUS_ERROR_INVALID_ID;
  }
  if (ownerref == NULL) {
    return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
  }

  if (global_mutex_arr[mutex]->state == RVCOS_MUTEX_STATE_UNLOCKED) {
    *ownerref = RVCOS_THREAD_ID_INVALID;
    return RVCOS_STATUS_SUCCESS;
  }
  else {
    *ownerref = global_mutex_arr[mutex]->owner;
    return RVCOS_STATUS_SUCCESS;
  }
}

TStatus RVCMutexAcquire(TMutexID mutex, TTick timeout) {
  if (!global_mutex_arr[mutex]) {
    return RVCOS_STATUS_ERROR_INVALID_ID;
  }
  if (timeout == RVCOS_TIMEOUT_IMMEDIATE &&
    global_mutex_arr[mutex]->state == RVCOS_MUTEX_STATE_LOCKED) {
    return RVCOS_STATUS_SUCCESS;
  }
  else if (timeout == RVCOS_TIMEOUT_IMMEDIATE) {
    global_mutex_arr[mutex]->state = RVCOS_MUTEX_STATE_LOCKED;
    return RVCOS_STATUS_SUCCESS;
  }
  else if (timeout == RVCOS_TIMEOUT_INFINITE &&
    global_mutex_arr[mutex]->state == RVCOS_MUTEX_STATE_UNLOCKED) {
    global_mutex_arr[mutex]->state == RVCOS_MUTEX_STATE_LOCKED;
    return RVCOS_STATUS_SUCCESS;
  }
  else if (timeout == RVCOS_TIMEOUT_INFINITE) {
    uint32_t thread_id =
      global_mutex_arr[mutex]->owner;  // block thread until mutex is acquired
    global_tcb_arr[thread_id]->state == RVCOS_THREAD_STATE_WAITING;

    return RVCOS_STATUS_SUCCESS;
  }
  else {
    while (timeout > 0) {
      if (global_mutex_arr[mutex]->state == RVCOS_MUTEX_STATE_UNLOCKED) {
        global_mutex_arr[mutex]->state = RVCOS_MUTEX_STATE_LOCKED;
        return RVCOS_STATUS_SUCCESS;
      }
      timeout -= 1;
    }
    return RVCOS_STATUS_FAILURE;
  }
}

TStatus RVCMutexRelease(TMutexID mutex) {
  if (!global_mutex_arr[mutex]) {
    return RVCOS_STATUS_ERROR_INVALID_ID;
  }
  if (global_mutex_arr[mutex]->owner != running_thread_id) {
    return RVCOS_STATUS_ERROR_INVALID_STATE;
  }

  global_mutex_arr[mutex]->owner = NULL;
  /*may cause another higher priority thread to be scheduled
  if it acquires the newly released mutex. */

  return RVCOS_STATUS_SUCCESS;
}

/**
 * @brief
 *
 * @param sp sp with malloced space
 * @param function
 * @param param
 * @param tp
 * @return uint32_t* the initialized sp
 */
uint32_t *initStack(uint32_t *sp, TEntry function, uint32_t param, uint32_t tp)
{
  sp--;
  *sp = (uint32_t)function; // sw      ra,48(sp)
  sp--;
  *sp = tp; // sw      tp,44(sp)
  sp--;
  *sp = 0; // sw      t0,40(sp)
  sp--;
  *sp = 0; // sw      t1,36(sp)
  sp--;
  *sp = 0; // sw      t2,32(sp)
  sp--;
  *sp = 0; // sw      s0,28(sp)
  sp--;
  *sp = 0; // sw      s1,24(sp)
  sp--;
  *sp = param; // sw      a0,20(sp)
  sp--;
  *sp = 0; // sw      a1,16(sp)
  sp--;
  *sp = 0; // sw      a2,12(sp)
  sp--;
  *sp = 0; // sw      a3,8(sp)
  sp--;
  *sp = 0; // sw      a4,4(sp)
  sp--;
  *sp = 0; // sw      a5,0(sp)
  return sp;
}