#include <stdint.h>

volatile int global = 42;
volatile uint32_t controller_status = 0;
volatile uint32_t *saved_sp;
volatile uint32_t *MainThread, *OtherThread;
volatile char *VIDEO_MEMORY = (volatile char *)(0x50000000 + 0xFE800);

typedef uint32_t (*TEntry)(uint32_t param);
typedef void (*TFunctionPointer)(void);

// Assembly functions
void enter_cartridge(void);
void ContextSwitch(volatile uint32_t **oldsp, volatile uint32_t *newsp);
uint32_t *InitStack(uint32_t *sp, TEntry fun, uint32_t param, uint32_t tp);
uint32_t call_on_other_gp(void *param, TEntry entry, uint32_t *gp);

#define CART_STAT_REG (*(volatile uint32_t *)0x4000001C) // addr is the num, cast to pointer, then deref

uint32_t MyThread(uint32_t param) {
  int last_global = 42;
  VIDEO_MEMORY[61] = '0';
  VIDEO_MEMORY[62] = '0';
  VIDEO_MEMORY[63] = '0';
  VIDEO_MEMORY[125] = '0';
  VIDEO_MEMORY[126] = '0';
  VIDEO_MEMORY[127] = '0';
  VIDEO_MEMORY[189] = '0';
  VIDEO_MEMORY[190] = '0';
  VIDEO_MEMORY[191] = '0';

  while (1) {
    if (global >= last_global + param) {
      ContextSwitch(&OtherThread, MainThread);
      VIDEO_MEMORY[63]++;
      if (VIDEO_MEMORY[63] > 'z') {
        VIDEO_MEMORY[63] = '0';
      }
      for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
          VIDEO_MEMORY[58 + j + 64 * i] = VIDEO_MEMORY[63];
        }
      }
      last_global = global;
    }
  }
  return 2; // arbitrary
}

int main() {
  saved_sp = &controller_status;
  int last_global = 42;

  int x_pos = 12;
  int param = 20;
  uint32_t OtherThreadStack[1024]; // 1024 is arbitrary

  while (1) {
    if (CART_STAT_REG & 0x1) {
      enter_cartridge();
    }

    if (global >= last_global + param) {
      if (controller_status) {
        VIDEO_MEMORY[x_pos] = ' ';
        if (controller_status & 0x1) {
          if (x_pos & 0x3F) {
            x_pos--;
          }
        }
        if (controller_status & 0x2) {
          if (x_pos >= 0x40) {
            x_pos -= 0x40;
          }
        }
        if (controller_status & 0x4) {
          if (x_pos < 0x8C0) {
            x_pos += 0x40;
          }
        }
        if (controller_status & 0x8) {
          if ((x_pos & 0x3F) != 0x3F) {
            x_pos++;
          }
        }
        VIDEO_MEMORY[x_pos] = 'X';
      }
      ContextSwitch(&MainThread, OtherThread);
      last_global = global;
    }
  }
  return 0;
}

uint32_t c_syscall_handler(uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4, uint32_t p5, uint32_t code) {
  return code + 1;
}