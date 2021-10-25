#ifndef RVCOS_H
#define RVCOS_H

#include <stdint.h>

#define RVCOS_STATUS_FAILURE                        ((TStatus)0x00)
#define RVCOS_STATUS_SUCCESS                        ((TStatus)0x01)
#define RVCOS_STATUS_ERROR_INVALID_PARAMETER        ((TStatus)0x02)
#define RVCOS_STATUS_ERROR_INVALID_ID               ((TStatus)0x03)
#define RVCOS_STATUS_ERROR_INVALID_STATE            ((TStatus)0x04)
#define RVCOS_STATUS_ERROR_INSUFFICIENT_RESOURCES   ((TStatus)0x05)

#define MAIN_THREAD_ID                              ((uint32_t)0x01)
#define IDLE_THREAD_ID                              ((uint32_t)0x00)

#define RVCOS_THREAD_STATE_CREATED                  ((TThreadState)0x01)
#define RVCOS_THREAD_STATE_DEAD                     ((TThreadState)0x02)
#define RVCOS_THREAD_STATE_RUNNING                  ((TThreadState)0x03)
#define RVCOS_THREAD_STATE_READY                    ((TThreadState)0x04)
#define RVCOS_THREAD_STATE_WAITING                  ((TThreadState)0x05)

#define RVCOS_THREAD_PRIORITY_IDLE                  ((TThreadPriority)0x00)
#define RVCOS_THREAD_PRIORITY_LOW                   ((TThreadPriority)0x01)
#define RVCOS_THREAD_PRIORITY_NORMAL                ((TThreadPriority)0x02)
#define RVCOS_THREAD_PRIORITY_HIGH                  ((TThreadPriority)0x03)

#define RVCOS_THREAD_ID_INVALID                     ((TThreadID)-1)

#define RVCOS_TIMEOUT_INFINITE                      ((TTick)0)
#define RVCOS_TIMEOUT_IMMEDIATE                     ((TTick)-1)

typedef uint32_t TStatus, *TStatusRef;
typedef uint32_t TTick, *TTickRef;
typedef int32_t  TThreadReturn, *TThreadReturnRef;
typedef uint32_t TMemorySize, *TMemorySizeRef;
typedef uint32_t TThreadID, *TThreadIDRef;
typedef uint32_t TThreadPriority, *TThreadPriorityRef;
typedef uint32_t TThreadState, *TThreadStateRef;
typedef char     TTextCharacter, *TTextCharacterRef;

typedef TThreadReturn (*TThreadEntry)(void *);

typedef struct{
    uint32_t DLeft:1;
    uint32_t DUp:1;
    uint32_t DDown:1;
    uint32_t DRight:1;
    uint32_t DButton1:1;
    uint32_t DButton2:1;
    uint32_t DButton3:1;
    uint32_t DButton4:1;
    uint32_t DReserved:28;
} SControllerStatus, *SControllerStatusRef;

TStatus RVCInitialize(uint32_t *gp);

TStatus RVCTickMS(uint32_t *tickmsref);
TStatus RVCTickCount(TTickRef tickref);

TStatus RVCThreadCreate(TThreadEntry entry, void *param, TMemorySize memsize, TThreadPriority prio, TThreadIDRef tid);
TStatus RVCThreadDelete(TThreadID thread);
TStatus RVCThreadActivate(TThreadID thread);
TStatus RVCThreadTerminate(TThreadID thread, TThreadReturn returnval);
TStatus RVCThreadWait(TThreadID thread, TThreadReturnRef returnref);
TStatus RVCThreadID(TThreadIDRef threadref);
TStatus RVCThreadState(TThreadID thread, TThreadStateRef stateref);
TStatus RVCThreadSleep(TTick tick);

TStatus RVCWriteText(const TTextCharacter *buffer, TMemorySize writesize);
TStatus RVCReadController(SControllerStatusRef statusref);


/**
 * @brief Implement below
 * 
 */

typedef struct _TCB {
  TThreadID thread_id;
  TThreadState state;
  TThreadPriority priority;
  TThreadEntry entry;

  uint32_t mem_size;
  void *sp;

  uint32_t *register_arr;

} TCB; 

volatile char *VIDEO_MEMORY = (volatile char *)(0x50000000 + 0xFE800);
volatile uint32_t *main_gp = 0;
volatile TCB **global_tcb_arr;
volatile int curr_tcb_index;
volatile int curr_active_thread;

TStatus RVCWriteText(const TTextCharacter *buffer, TMemorySize writesize)
{
  const uint32_t stat = 0;
  for (int i = 0; i < writesize; i++)
  {
    VIDEO_MEMORY[i] = buffer[i];
  }

  return stat;
}

TStatus RVCInitialize(uint32_t *gp){
  if (main_gp){
    return RVCOS_STATUS_ERROR_INVALID_STATE;
  }

  global_tcb_arr = malloc(sizeof(TCB) * 256);

  //Creating IDLE thread and IDLE thread TCB
  TCB *idle_thread_tcb = malloc(sizeof(TCB));
  global_tcb_arr[curr_tcb_index] = idle_thread_tcb;
  curr_tcb_index++;

  idle_thread_tcb->priority = RVCOS_THREAD_PRIORITY_IDLE;
  idle_thread_tcb->thread_id = IDLE_THREAD_ID;
  idle_thread_tcb->state = RVCOS_THREAD_STATE_CREATED;
  idle_thread_tcb->mem_size = 1024;
  idle_thread_tcb->sp = malloc(idle_thread_tcb->mem_size) + idle_thread_tcb->mem_size;
  // idle_thread_tcb->entry = 

  //Creating MAIN thread and MAIN thread TCB
  TCB * main_thread_tcb = malloc(sizeof(TCB));
  global_tcb_arr[curr_tcb_index] = main_thread_tcb;
  curr_tcb_index++;

  main_thread_tcb->thread_id = MAIN_THREAD_ID;
  main_thread_tcb->state = RVCOS_THREAD_STATE_RUNNING;
  main_gp = gp;

  return RVCOS_STATUS_SUCCESS;
}

void idleFunction(){
  asm volatile("csrci mstatus, 0x8");
  while (1)
  {

  }
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
TStatus RVCThreadCreate(TThreadEntry entry, void *param, TMemorySize memsize, TThreadPriority prio, TThreadIDRef tid) {
  /**
   * everything is in global app space
   * 
   * allocate mem on the main app heap
   * 
   * 
   * 
   * make tcb
   * set priority in TCB
   * set thread id
   * set state to RVCOS_THREAD_STATE_CREATED
   * 
   * */
  if (!entry || !tid) {
    return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
  }

  char *tid_text = "thread create";
  // itoa(tid, tid_text, 10);
  RVCWriteText(tid_text, strlen(tid_text));

  TCB *curr_thread_tcb = malloc(sizeof(TCB));
  void *thread_memory = malloc(memsize);

  curr_thread_tcb->priority = prio;
  curr_thread_tcb->state = RVCOS_THREAD_STATE_CREATED;
  curr_thread_tcb->thread_id = curr_tcb_index;
  curr_thread_tcb->register_arr = malloc(13 * 4); // 13 registers

  if (curr_tcb_index < 255) {
    global_tcb_arr[curr_tcb_index] = curr_thread_tcb;
    curr_tcb_index++;
    *tid = curr_tcb_index;
  }
  else
  {
    return RVCOS_STATUS_FAILURE;
  }

  /**
   * switch case here for diff priorities
   * 
   */

  return RVCOS_STATUS_SUCCESS;
}

TStatus RVCThreadActivate(TThreadID thread) {
  // init stack here
  
}

TStatus RVCThreadID(TThreadIDRef threaddref) {

  if (!threaddref) {
    return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
  }

  *threaddref = curr_active_thread;
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

#endif
