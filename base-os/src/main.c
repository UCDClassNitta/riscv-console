#include <stdint.h>
#include "RVCOS.h"
volatile int global = 42;
volatile uint32_t controller_status = 0;
volatile uint32_t *saved_sp;
volatile uint32_t *MainThread, *OtherThread;

typedef uint32_t (*TEntry)(uint32_t param);
typedef void (*TFunctionPointer)(void);

// Assembly functions
void enter_cartridge(void);
void ContextSwitch(volatile uint32_t **oldsp, volatile uint32_t *newsp);
uint32_t *InitStack(uint32_t *sp, TEntry fun, uint32_t param, uint32_t tp);
uint32_t call_on_other_gp(void *param, TEntry entry, uint32_t *gp);

#define CART_STAT_REG (*(volatile uint32_t *)0x4000001C) // addr is the num, cast to pointer, then deref

int main() {
  saved_sp = &controller_status;
  int last_global = 42;

  int x_pos = 12;
  int param = 20;
  //uint32_t OtherThreadStack[1024]; // 1024 is arbitrary


  while (1) {
    if (CART_STAT_REG & 0x1) {
      enter_cartridge();
    }
    // if (CART_STAT_REG & ??){

    // }

  }
  return 0;
}

/**
 * @brief
 *
 * @param p1 param1
 * @param p2 param2
 * @param p3 ...
 * @param p4
 * @param p5
 * @param syscall_code .. switch case on this to call the corresponding functions
 * @return uint32_t
 */
uint32_t c_syscall_handler(uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4, uint32_t p5, uint32_t syscall_code) {

  switch (syscall_code) {
    case 0: {
      RVCInitialize(p1);
      break;
    }
    case 1: {
      RVCThreadCreate(p1, p2, p3, p4, p5);
      break;
    }
    case 2: {
      RVCThreadDelete(p1);
      break;
    }
    case 3: {
      RVCThreadActivate(p1);
      break;
    }
    case 4: {
      RVCThreadTerminate(p1, p2);
      break;
    }
    case 5: {
      RVCThreadWait(p1, p2);
      break;
    }
    case 6: {
      RVCThreadID(p1);
      break;
    }
    case 7: {
      RVCThreadState(p1, p2);
      break;
    }
    case 8: {
      RVCThreadSleep(p1);
      break;
    }
    case 9: {
      RVCTickMS(p1);
      break;
    }
    case 10: {
      RVCTickCount(p1);
      break;
    }
    case 11: {
      RVCWriteText(p1, p2);
      break;
    }
    // case 12: {
    //   RVCReadController(p1);
    // }
  default:
    break;
  }


  return syscall_code + 1; // plus 1 just to make a change
}

/**
 * @brief 
 * 
 * @param sp 
 * @param function 
 * @param param 
 * @param tp 
 * @return uint32_t* the initialized sp
 */
uint32_t *InitStack(uint32_t *sp, TEntry function, uint32_t param, uint32_t tp)
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