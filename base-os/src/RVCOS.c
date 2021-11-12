#include "RVCOS.h"
#include "queues.h"
#include "esc_codes.h"

volatile char *VIDEO_MEMORY = (volatile char *)(0x50000000 + 0xFE800);
volatile uint32_t *main_gp = 0;

TCB **global_tcb_arr = NULL;
uint32_t curr_available_mem_pool_index = 0;
MemoryPoolController *global_mem_pool_arr = NULL; // TODO change the 10

PriorityQueue *low_prio;
PriorityQueue *med_prio;
PriorityQueue *high_prio;
PriorityQueue *idle_prio;
PriorityQueue *wait_q;

volatile uint32_t last_write_pos = 0;
volatile uint32_t running_thread_id = 1;

uint32_t call_on_other_gp(void *param, TEntry entry, uint32_t *gp);

void threadSkeleton(uint32_t thread)
{
  asm volatile("csrsi mstatus, 0x8"); // enable interrupts

  // call entry(param) but make sure to switch the gp right before the call

  uint32_t ret_val = call_on_other_gp(global_tcb_arr[thread]->param, global_tcb_arr[thread]->entry, main_gp);
  RVCThreadTerminate(thread, ret_val);
}

void schedule()
{
  uint32_t old_id = running_thread_id;
  uint32_t new_id = 0;
  if (high_prio->size > 0)
  {
    dequeue(high_prio, &new_id);
    running_thread_id = new_id;
  }
  else if (med_prio->size > 0)
  {
    dequeue(med_prio, &new_id);
    running_thread_id = new_id;
  }
  else if (low_prio->size > 0)
  {
    dequeue(low_prio, &new_id);
    running_thread_id = new_id;
  }
  else
  {
    running_thread_id = 0; // goto idle
  }
  ContextSwitch(global_tcb_arr[old_id]->sp, global_tcb_arr[new_id]->sp);
}

/**
 * @brief Writes a string to the screen
 *
 * @param str string to write
 */
void WriteString(const char *str)
{
  const char *Ptr = str;
  while (*Ptr)
  {
    Ptr++;
  } // compute size
  RVCWriteText(str, Ptr - str);
}

void writeInt(uint32_t val)
{
  char *thread_text[33];
  itoa(val, thread_text, 10);
  WriteString(thread_text);
}

void idleFunction()
{
  asm volatile("csrci mstatus, 0x8"); // enables interrupts
  while (1)
  {
    ;
  }
}

// TODO: Update this to remove 256 cap
uint32_t getNextAvailableTCBIndex()
{
  for (uint32_t i = 0; i < 255; i++)
  {
    if (!global_tcb_arr[i])
    { // if the curr slot is empty
      return i;
    }
  }
  return -1; // no available slots
}

uint32_t getNextAvailableMemPoolIndex()
{
  WriteString("next available mem id: ");
  writeInt(curr_available_mem_pool_index);
  curr_available_mem_pool_index++;
  return curr_available_mem_pool_index - 1;
}

// TODO check the order here
PriorityQueue *getPQByPrioNum(uint32_t prio_num)
{
  switch (prio_num)
  {
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

// ! change the logic of queues here
TStatus RVCInitialize(uint32_t *gp)
{
  if (!gp)
  {
    return RVCOS_STATUS_ERROR_INVALID_STATE;
  }
  main_gp = gp;

  global_tcb_arr = malloc(sizeof(TCB *) * 256); // TODO: remove 256 cap
  for (uint32_t i = 0; i < 256; i++)
  {
    global_tcb_arr[i] = NULL;
  } // manual calloc

  // TODO: change all of this to linked list implementation
  // Init all PQs
  low_prio = (PriorityQueue *)malloc(sizeof(PriorityQueue));
  med_prio = (PriorityQueue *)malloc(sizeof(PriorityQueue));
  high_prio = (PriorityQueue *)malloc(sizeof(PriorityQueue));
  idle_prio = (PriorityQueue *)malloc(sizeof(PriorityQueue));

  low_prio->head = med_prio->head = high_prio->head = idle_prio->head = NULL;
  low_prio->tail = med_prio->tail = high_prio->tail = idle_prio->tail = NULL;
  low_prio->size = med_prio->size = high_prio->size = idle_prio->size = 0;

  // WriteString("Low prio pointer: ");
  // WriteInt((uint32_t)low_prio);
  // WriteString("\n");
  //  Creating IDLE thread and IDLE thread TCB
  uint32_t *idle_tid = malloc(sizeof(uint32_t));
  // create handles putting it in TCB[]
  RVCThreadCreate(idleFunction, NULL, 1024, RVCOS_THREAD_PRIORITY_IDLE, idle_tid);
  free(idle_tid);
  // WriteString("made idle thread");

  // Creating MAIN thread and MAIN thread TCB manually because it's a special case
  TCB *main_thread_tcb = malloc(sizeof(TCB));
  main_thread_tcb->thread_id = MAIN_THREAD_ID;

  // WriteString("main id: ");
  // WriteInt(main_thread_tcb->thread_id);
  // WriteString("\n");

  main_thread_tcb->state = RVCOS_THREAD_STATE_RUNNING;
  main_thread_tcb->sp = 0x71000000;     // top of physical stack
  main_thread_tcb->mem_size = 0xE00000; //? is this 14MB
  main_thread_tcb->priority = 2;

  global_tcb_arr[1] = main_thread_tcb;

  return RVCOS_STATUS_SUCCESS;
}

TStatus RVCThreadDelete(TThreadID thread)
{
  if (!global_tcb_arr[thread])
  {
    return RVCOS_STATUS_ERROR_INVALID_ID;
  }

  if (global_tcb_arr[thread]->state != RVCOS_THREAD_STATE_DEAD)
  {
    return RVCOS_STATUS_ERROR_INVALID_STATE;
  }

  uint32_t tid; // here just for the function param, not used
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

TStatus RVCThreadCreate(TThreadEntry entry, void *param, TMemorySize memsize, TThreadPriority prio, TThreadIDRef tid)
{
  if (!tid)
  {
    // WriteInt(tid);
    // WriteString("bad tid\n");
  }

  if (!entry || !tid)
  {
    return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
  }

  TCB *curr_thread_tcb = malloc(sizeof(TCB));
  void *thread_memory = malloc(memsize);

  curr_thread_tcb->priority = prio;
  curr_thread_tcb->state = RVCOS_THREAD_STATE_CREATED;
  curr_thread_tcb->mem_size = memsize;
  curr_thread_tcb->entry = entry;
  curr_thread_tcb->param = param;

  *tid = getNextAvailableTCBIndex();
  // WriteString("tid is: ");
  // WriteInt(*tid);
  if (*tid == -1)
  {
    return RVCOS_STATUS_FAILURE;
  }
  else
  {
    curr_thread_tcb->thread_id = *tid;
    global_tcb_arr[*tid] = curr_thread_tcb;
  }

  enqueue(getPQByPrioNum(prio), tid);
  return RVCOS_STATUS_SUCCESS;
}

TStatus RVCThreadActivate(TThreadID thread)
{
  /**
   * Init stack here
   * save all reg
   *
   */

  char *thread_text[100];

  if (global_tcb_arr[thread] == NULL)
  {
    return RVCOS_STATUS_ERROR_INVALID_ID;
  }

  uint32_t state = global_tcb_arr[thread]->state;
  if (state == RVCOS_THREAD_STATE_CREATED || state == RVCOS_THREAD_STATE_DEAD)
  {
    return RVCOS_STATUS_ERROR_INVALID_STATE;
  }

  global_tcb_arr[thread]->sp = malloc(global_tcb_arr[thread]->mem_size);

  global_tcb_arr[thread]->state = RVCOS_THREAD_STATE_READY;

  uint32_t tid;
  dequeue(getPQByPrioNum(global_tcb_arr[thread]->priority), &tid);
  //  set thread to STATUS_RUNNING
  running_thread_id = tid;
  // TODO  run thread from entry point

  return RVCOS_STATUS_SUCCESS;
}

TStatus RVCThreadTerminate(TThreadID thread, TThreadReturn returnval)
{
  if (!global_tcb_arr[thread])
  {
    return RVCOS_STATUS_ERROR_INVALID_ID;
  }

  uint32_t state = global_tcb_arr[thread]->state;
  if (state == RVCOS_THREAD_STATE_CREATED || state == RVCOS_THREAD_STATE_DEAD)
  {
    return RVCOS_STATUS_ERROR_INVALID_STATE;
  }

  global_tcb_arr[thread]->state = RVCOS_THREAD_STATE_DEAD;
  returnval = global_tcb_arr[thread]->ret_val;

  // call scheduler
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
TStatus RVCWriteText(const TTextCharacter *buffer, TMemorySize writesize)
{
  const uint32_t stat = 0;
  uint32_t n_pos = 0;
  uint32_t physical_write_pos = last_write_pos;

  // For escape codes
  uint32_t row_int;
  uint32_t col_int;

  for (uint32_t j = 0; j < writesize; j++)
  {
    if ((uint32_t)buffer[j + 2] == '\x1B' && (uint32_t)buffer[j + 3] == '[') {
      if (writesize > 7) {
        char row[3];
        char col[3];

        // These variables will probably need to be declared in a broader scope when integrated
        uint32_t row_int;
        uint32_t col_int;

        if ((uint32_t)buffer[j + 5] == ';') { //Row is single digit
            strncpy(row, (buffer + j) + 4, 1);
            row[1] = '\0';
            row_int = atoi(row);

            if ((uint32_t)buffer[j + 7] == 'H') { //Col is single digit
                strncpy(col, (buffer + j) + 6, 1);
                col[1] = '\0';
                col_int = atoi(col);
            }
            else if ((uint32_t)buffer[j + 8] == 'H') { //Col is double digit
                strncpy(col, (buffer + j) + 6, 2);
                col[2] = '\0';
                col_int = atoi(col);;
            }
            else {break;}
        }
        else if ((uint32_t)buffer[j + 6] == ';') { //Row is double digit
            strncpy(row, (buffer + j) + 4, 2);
            row[2] = '\0';
            row_int = atoi(row);

            if ((uint32_t)buffer[j + 8] == 'H') { //Col is single digit
                strncpy(col, (buffer + j) + 7, 1);
                col[1] = '\0';
                col_int = atoi(col);
            }
            else if ((uint32_t)buffer[j + 9] == 'H') { //Col is double digit
                strncpy(col, (buffer + j) + 7, 2);
                col[2] = '\0';
                col_int = atoi(col);
            }
            else {break;}
        }

        // Final check to ensure integer values are within range
        uint32_t row_check = (row_int >= 0 && row_int < 36) ? 1 : 0;
        uint32_t col_check = (col_int >= 0 && col_int < 64) ? 1 : 0;

        if (!row_check || !col_check)
        {
          WriteString("Invalid Parameters");
        }
        else
        {
          WriteString("Moving cursor to specified row and column");
          // Code to move cursor to row and column
          // BE CAREFUL of 'off by one' errors here
          // It's assumed escape code will use 0-35 for rows and 0-63 for columns
        }
      }

      // Sent to external esc_codes switch
      else
      {
        uint32_t code = esc_codes(buffer, writesize);

        switch (code)
        {
        case 1:
        {

          // code to move cursor up
          break;
        }
        case 2:
        {
          RVCWriteText("\b \n", 3);
          RVCWriteText("X", 1);
          // code to move cursor down
          break;
        }
        case 3:
        {
          RVCWriteText("\b ", 2);
          RVCWriteText("X", 1);
          // code to move cursor right
          break;
        }
        case 4:
        {
          RVCWriteText("\b \b\b", 4);
          RVCWriteText("X", 1);
          // code to move cursor left
          break;
        }
        case 5:
        {
          WriteString("H: Cursor Upper Left");
          // code to move cursor upper left
          break;
        }
        case 6:
        {
          WriteString("2: Erase screen, leave cursor");
          // code to erase and leave cursor
          break;
        }
        default:
          break;
        }
      }
      return stat;
    }
    if (buffer[j] == '\n')
    {
      uint32_t next_line = (physical_write_pos / 64) + 1;
      physical_write_pos = next_line * 64;
      physical_write_pos -= j; // now j is not 0 anymore, so push physical_write_pos back by j
      n_pos = j - 1;
    }
    else if (buffer[j] == '\b')
    {
      physical_write_pos = physical_write_pos - 2 > 0 ? physical_write_pos - 2 : 0; // make sure writepos is always >=0
    }
    VIDEO_MEMORY[physical_write_pos++] = buffer[j];
  }

  // change this line to change the behavior of writing to a filled screen/
  // now it just goes back to 0 and overwrites what's on screen
  last_write_pos = (physical_write_pos + n_pos) % MAX_VRAM_INDEX;

  return stat;
}

TStatus RVCThreadID(TThreadIDRef threaddref)
{
  if (!threaddref)
  {
    return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
  }
  *threaddref = running_thread_id;
  return RVCOS_STATUS_SUCCESS;
}

TStatus RVCThreadState(TThreadID thread, TThreadStateRef state)
{
  if (!global_tcb_arr[thread])
  {
    return RVCOS_STATUS_ERROR_INVALID_ID;
  }
  if (!state)
  {
    return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
  }

  WriteString("the running thread state is: ");
  writeInt(global_tcb_arr[thread]->state);
  *state = global_tcb_arr[thread]->state;
  return RVCOS_STATUS_SUCCESS;
}

TStatus RVCThreadWait(TThreadID thread, TThreadReturnRef returnref)
{

  if (!global_tcb_arr[thread])
  {
    return RVCOS_STATUS_ERROR_INVALID_ID;
  }
  if (returnref == NULL)
  {
    return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
  }

  return RVCOS_STATUS_SUCCESS;
}

TStatus RVCThreadSleep(TTick tick)
{
  return RVCOS_STATUS_SUCCESS;
}

TStatus RVCTickMS(uint32_t *tickmsref)
{
  if (tickmsref)
  {
    uint32_t time = TIME_REG * 1000;
    *tickmsref = time;
    return RVCOS_STATUS_SUCCESS;
  }
  else
  {
    return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
  }

  return RVCOS_STATUS_SUCCESS;
}

//? what does this function do?
TStatus RVCTickCount(TTickRef tickref)
{
  if (tickref)
  {
    uint32_t time = TIME_REG;
    *tickref = time;
    return RVCOS_STATUS_SUCCESS;
  }
  else
  {
    return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
  }
}

TStatus RVCReadController(SControllerStatusRef statusref)
{
  if (!statusref)
  {
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

  statusref->DReserved = CONTROLLER_REG_VAL >> 8; // no need to AND b/c we want all 24 bits

  return RVCOS_STATUS_SUCCESS;
}

TStatus RVCMemoryPoolCreate(void *base, TMemorySize size, TMemoryPoolIDRef memoryref)
{
  WriteString("creating pool\n");
  if (base == NULL || size < 2 * MIN_ALLOC_SIZE || memoryref == NULL)
  {
    WriteString("bad params\n");
    return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
  }
  *memoryref = getNextAvailableMemPoolIndex();

  WriteString("mem id is: \n");
  writeInt(*memoryref);
  WriteString("\n");

  MemoryPoolController pool;

  pool.first_chunk = base + sizeof(MemoryPoolController); // base->||controller||chunk|data||
  pool.pool_id = *memoryref;
  pool.pool_size = size;
  pool.bytes_left = size;

  pool.first_chunk->isFree = 1;
  pool.first_chunk->data_size = size;
  pool.first_chunk->next = NULL;
  pool.first_chunk->prev = NULL;

  global_mem_pool_arr[*memoryref] = pool;

  writeInt(size);
  WriteString("Current Pool addr: ");
  writeInt(global_mem_pool_arr[*memoryref].first_chunk);
  WriteString("\n");

  WriteString("Pool Id: ");
  writeInt(pool.pool_id);
  WriteString("\n");

  WriteString("Pool size: ");
  writeInt(global_mem_pool_arr[*memoryref].pool_size);
  WriteString("\n");

  WriteString("Pool left: ");
  writeInt(global_mem_pool_arr[*memoryref].bytes_left);
  WriteString("\n");
  return RVCOS_STATUS_SUCCESS;
}
TStatus RVCMemoryPoolDelete(TMemoryPoolID memory)
{
  if (memory < 0)
  {
    return RVCOS_STATUS_ERROR_INVALID_ID;
  }
  if (global_mem_pool_arr[memory].first_chunk == NULL)
  {
    WriteString("Successfully freed.\n");
    free(global_mem_pool_arr[memory].first_chunk - sizeof(MemoryPoolController));
    return RVCOS_STATUS_SUCCESS;
  }
  else
  {
    WriteString("Still some memory left.\n");
    return RVCOS_STATUS_ERROR_INVALID_STATE;
  }
}
TStatus RVCMemoryPoolQuery(TMemoryPoolID memory, TMemorySizeRef bytesleft)
{
  if (memory < 0)
  {
    WriteString("bad mem id\n");
    return RVCOS_STATUS_ERROR_INVALID_ID;
  }

  *bytesleft = global_mem_pool_arr[memory].bytes_left;
  WriteString("Pool Bytes Remaining: ");
  writeInt(global_mem_pool_arr[memory].bytes_left);
  WriteString("\n");

  return RVCOS_STATUS_SUCCESS;
}

/**
 * @brief Allocates space from a given Memory Pool
 *
 * @param memory
 * @param size
 * @param pointer
 * @return TStatus
 *
 * @Notes
 * This assumes @param memory is a already initialized mem pool except when memory == 0
 */
TStatus RVCMemoryPoolAllocate(TMemoryPoolID memory, TMemorySize size, void **pointer)
{
  if (memory < 0)
  {
    WriteString("bad mem id\n");
    return RVCOS_STATUS_ERROR_INVALID_ID;
  }
  if (!size || !pointer)
  {
    WriteString("bad pointer or size\n");
    return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
  }

  if (memory == RVCOS_MEMORY_POOL_ID_SYSTEM && global_mem_pool_arr == NULL)
  {
    WriteString("Making Pool for system\n");
    uint32_t max_chunk_count = (size / MIN_ALLOC_SIZE) > 2 ? (size / MIN_ALLOC_SIZE) : 2;
    *pointer = malloc(size + sizeof(MemoryPoolController) + sizeof(MemoryChunk) * max_chunk_count);
    global_mem_pool_arr = malloc(10 * sizeof(MemoryPoolController *));
    WriteString("malloc worked\n");

    return RVCOS_STATUS_SUCCESS;
  }

  // do some normal mem pool stuff
  MemoryPoolController pool = global_mem_pool_arr[memory];
  if (!&pool)
  {
    WriteString("This pool doesn't exist: ");
    writeInt(memory);
    WriteString("\n");
    return RVCOS_STATUS_ERROR_INVALID_ID;
  }
  TMemorySize actual_alloc_size = ((size % 64) + 1) * 64;

  MemoryChunk *curr_chunk = pool.first_chunk;
  while (curr_chunk)
  {
    if (curr_chunk->isFree && curr_chunk->data_size >= actual_alloc_size)
    {

      // mark current as an occupied chunk
      curr_chunk->isFree = 0;
      *pointer = curr_chunk + sizeof(MemoryChunk); // move offset to actual mem location

      // now make the free chunk
      // ! chueck if it's in bounds
      MemoryChunk *free_chunk = curr_chunk + sizeof(MemoryChunk) + actual_alloc_size;
      free_chunk->isFree = 1;
      free_chunk->prev = curr_chunk;
      free_chunk->next = curr_chunk->next;
      free_chunk->data_size = curr_chunk->data_size - sizeof(MemoryChunk) - actual_alloc_size; // TODO: check if this expression is correct

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
 * @param memory
 * @param pointer
 * @return TStatus
 */
TStatus RVCMemoryPoolDeallocate(TMemoryPoolID memory, void *pointer)
{
  if (memory < 0)
  {
    return RVCOS_STATUS_ERROR_INVALID_ID;
  }
  if (!pointer)
  {
    return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
  }

  MemoryPoolController pool = global_mem_pool_arr[memory];
  MemoryChunk *chunk_to_return = pointer - sizeof(MemoryChunk);

  chunk_to_return->prev->data_size += chunk_to_return->data_size + sizeof(MemoryChunk);
  chunk_to_return->prev->next = chunk_to_return->next;
  chunk_to_return->next->prev = chunk_to_return->prev;

  return RVCOS_STATUS_SUCCESS;
}

/** Mutex Functions **/

TStatus RVCMutexCreate(TMutexIDRef mutexref)
{
  return RVCOS_STATUS_SUCCESS;
}
TStatus RVCMutexDelete(TMutexID mutex)
{
  return RVCOS_STATUS_SUCCESS;
}
TStatus RVCMutexQuery(TMutexID mutex, TThreadIDRef ownerref)
{
  return RVCOS_STATUS_SUCCESS;
}
TStatus RVCMutexAcquire(TMutexID mutex, TTick timeout)
{
  return RVCOS_STATUS_SUCCESS;
}
TStatus RVCMutexRelease(TMutexID mutex)
{
  return RVCOS_STATUS_SUCCESS;
}