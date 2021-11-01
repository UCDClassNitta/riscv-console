#ifndef RVCOS_H
#define RVCOS_H

#include <stdint.h>
#include <stdlib.h>

#define RVCOS_STATUS_FAILURE ((TStatus)0x00)
#define RVCOS_STATUS_SUCCESS ((TStatus)0x01)
#define RVCOS_STATUS_ERROR_INVALID_PARAMETER ((TStatus)0x02)
#define RVCOS_STATUS_ERROR_INVALID_ID ((TStatus)0x03)
#define RVCOS_STATUS_ERROR_INVALID_STATE ((TStatus)0x04)
#define RVCOS_STATUS_ERROR_INSUFFICIENT_RESOURCES ((TStatus)0x05)

#define MAIN_THREAD_ID ((uint32_t)0x01)
#define IDLE_THREAD_ID ((uint32_t)0x00)

#define RVCOS_THREAD_STATE_CREATED ((TThreadState)0x01)
#define RVCOS_THREAD_STATE_DEAD ((TThreadState)0x02)
#define RVCOS_THREAD_STATE_RUNNING ((TThreadState)0x03)
#define RVCOS_THREAD_STATE_READY ((TThreadState)0x04)
#define RVCOS_THREAD_STATE_WAITING ((TThreadState)0x05)
#define RVCOS_THREAD_STATE_SLEEP ((TThreadSTate)0x06)

#define RVCOS_THREAD_PRIORITY_IDLE ((TThreadPriority)0x00)
#define RVCOS_THREAD_PRIORITY_LOW ((TThreadPriority)0x01)
#define RVCOS_THREAD_PRIORITY_NORMAL ((TThreadPriority)0x02)
#define RVCOS_THREAD_PRIORITY_HIGH ((TThreadPriority)0x03)

#define RVCOS_THREAD_ID_INVALID ((TThreadID)-1)

#define RVCOS_TIMEOUT_INFINITE ((TTick)0)
#define RVCOS_TIMEOUT_IMMEDIATE ((TTick)-1)

#define TIME_REG (*(volatile uint32_t *)0x40000008)       // already derefed, ready to use
#define CONTROLLER_REG_VAL (*(volatile uint32_t *)0x40000018) // already derefed

typedef uint32_t TStatus, *TStatusRef;
typedef uint32_t TTick, *TTickRef;
typedef int32_t TThreadReturn, *TThreadReturnRef;
typedef uint32_t TMemorySize, *TMemorySizeRef;
typedef uint32_t TThreadID, *TThreadIDRef;
typedef uint32_t TThreadPriority, *TThreadPriorityRef;
typedef uint32_t TThreadState, *TThreadStateRef;
typedef char TTextCharacter, *TTextCharacterRef;
typedef uint32_t (*TEntry)(uint32_t param);

typedef TThreadReturn (*TThreadEntry)(void *);

typedef struct
{
  uint32_t DLeft : 1; // num after colon means how many bits this field has
  uint32_t DUp : 1;
  uint32_t DDown : 1;
  uint32_t DRight : 1;
  uint32_t DButton1 : 1;
  uint32_t DButton2 : 1;
  uint32_t DButton3 : 1;
  uint32_t DButton4 : 1;   // 1 bit field
  uint32_t DReserved : 24; // 24 bit field
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
 * Helper functions
 */
void WriteString(const char *str);
void WriteInt(const uint32_t num);
void idleFunction();
void enqueue(uint32_t id, uint32_t target_prio);
uint32_t dequeue(uint32_t target_prio);
void thread_skeleton(uint32_t thread);

typedef struct _TCB
{
  TThreadID thread_id;
  TThreadState state;
  TThreadPriority priority;
  TThreadEntry entry;
  TThreadReturn ret_val;

  uint32_t mem_size;
  void *param;
  void *sp;
  uint32_t sleep_tick; // decrement this if not null and state is sleeping
} TCB;

typedef struct _PriorityQueue
{
  uint32_t size;
  uint32_t *queue;
  uint32_t head;
  uint32_t tail;
} PriorityQueue;

#endif
