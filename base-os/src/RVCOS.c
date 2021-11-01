#include "RVCOS.h"

volatile char *VIDEO_MEMORY = (volatile char *)(0x50000000 + 0xFE800);
volatile uint32_t *main_gp = 0;

volatile uint32_t available_tcb_index;

TCB **global_tcb_arr;

PriorityQueue *low_prio;
PriorityQueue *med_prio;
PriorityQueue *high_prio;
PriorityQueue *wait_q;

volatile uint32_t write_line_index = 0;
volatile uint32_t last_write_pos = 0;
volatile uint32_t running_thread_id = 1;

uint32_t call_on_other_gp(void *param, TEntry entry, uint32_t *gp);
void thread_skeleton(uint32_t thread);

void thread_skeleton(uint32_t thread)
{

  // Do any setup for making tp = thread_id
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
    new_id = dequeue(3);
    running_thread_id = new_id;
  }
  else if (med_prio->size > 0)
  {
    new_id = dequeue(2);
    running_thread_id = new_id;
  }
  else if (low_prio->size > 0)
  {
    new_id = dequeue(1);
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

void WriteInt(const uint32_t num)
{
  char *thread_text[100];
  WriteString(itoa(num, thread_text, 10));
}

void idleFunction()
{
  asm volatile("csrci mstatus, 0x8"); // enables interrupts
  while (1)
  {
    ;
  }
}

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

// TODO: this is using stack behaviour, change back to queue
// Add thread to respective queue
// ! this assumes the target queue is never full
void enqueue(uint32_t id, uint32_t target_prio)
{

  PriorityQueue *target;
  switch (target_prio)
  {
  case 1:
  {
    target = low_prio;
    break;
  }
  case 2:
  {
    target = med_prio;
    break;
  }
  case 3:
  {
    target = high_prio;
    break;
  }
  }

  if (target->size < 256)
  {
    target->tail++;
    target->queue[target->tail] = id; // insert at tail
    target->size++;
  }
}

// Remove thread from respective queue by @param id
// ! this assumes the target queue is never full
uint32_t dequeue(uint32_t target_prio)
{
  PriorityQueue *target;
  switch (target_prio)
  {
  case 1:
  {
    target = low_prio;
    break;
  }
  case 2:
  {
    target = med_prio;
    break;
  }
  case 3:
  {
    target = high_prio;
    break;
  }
  }

  uint32_t old_id = 1;
  if (target->size > 0)
  {
    target->tail--;
    uint32_t old_id = target->queue[target->tail];
    free(global_tcb_arr[old_id]);
    target->queue[target->tail] = NULL;
    target->size--;
  }
  return old_id;
}

TStatus RVCInitialize(uint32_t *gp)
{
  if (!gp)
  {
    return RVCOS_STATUS_ERROR_INVALID_STATE;
  }
  main_gp = gp;

  global_tcb_arr = malloc(sizeof(TCB *) * 256);

  // init all PQs
  low_prio = (PriorityQueue *)malloc(sizeof(PriorityQueue));
  low_prio->queue = malloc(sizeof(uint32_t) * 256);
  low_prio->head = 0;
  low_prio->tail = 0;

  med_prio = (PriorityQueue *)malloc(sizeof(PriorityQueue));
  med_prio->queue = malloc(sizeof(uint32_t) * 256);
  med_prio->head = 0;
  med_prio->tail = 0;

  high_prio = (PriorityQueue *)malloc(sizeof(PriorityQueue));
  high_prio->queue = malloc(sizeof(uint32_t) * 256);
  high_prio->head = 0;
  high_prio->tail = 0;

  // Creating IDLE thread and IDLE thread TCB
  uint32_t *idle_tid;
  // ! create handles putting it in TCB[]
  RVCThreadCreate(idleFunction, NULL, 1024, RVCOS_THREAD_PRIORITY_IDLE, idle_tid);

  // Creating MAIN thread and MAIN thread TCB manually because it's a special case
  TCB *main_thread_tcb = malloc(sizeof(TCB));
  main_thread_tcb->thread_id = MAIN_THREAD_ID;
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

  dequeue(global_tcb_arr[thread]->priority);
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
  if (*tid == -1)
  {
    return RVCOS_STATUS_FAILURE;
  }
  else
  {
    curr_thread_tcb->thread_id = *tid;
    global_tcb_arr[*tid] = curr_thread_tcb;
  }
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
  uint32_t prio = global_tcb_arr[thread]->priority;
  // dequeue(thread, prio);
  //  set thread to STATUS_RUNNING
  //  run thread from entry point

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

  uint32_t physical_write_pos = last_write_pos;

  for (uint32_t j = 0; j < writesize; j++) // for each char
  {
    if (buffer[j] == '\n')
    {
      uint32_t next_line = (physical_write_pos / 64) + 1;
      physical_write_pos = next_line * 64;
      physical_write_pos -= j-1; // now j is not 0 anymore, so push physical_write_pos back by j
    }
    else if (buffer[j] == '\b')
    {
      physical_write_pos = physical_write_pos - 2 > 0 ? physical_write_pos - 2 : 0; // make sure writepos is always >=0
    }
    VIDEO_MEMORY[physical_write_pos++] = buffer[j];
  }

  last_write_pos = (physical_write_pos) % MAX_VRAM_INDEX;
 
  return stat;
}

TStatus RVCThreadID(TThreadIDRef threaddref)
{
  if (!threaddref)
  {
    return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
  }

  threaddref = global_tcb_arr[running_thread_id];
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
