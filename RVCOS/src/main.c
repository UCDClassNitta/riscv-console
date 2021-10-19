#include <stdint.h>
#include "RVCOS.h"
volatile int global;
volatile uint32_t controller_status = 0;
volatile uint32_t *saved_sp;
volatile uint32_t global_p;
typedef void (*TFunctionPointer)(void);
void enter_cartridge(void);
#define CART_STAT_REG (*(volatile uint32_t *)0x4000001C)
volatile char *VIDEO_MEMORY = (volatile char *)(0x50000000 + 0xFE800);  // taken from riscv-example, main code

volatile struct TCB* threadArray[256]; 

struct TCB{
    int tid;
    uint32_t *gp;
    int state; // different states: running, ready, dead, waiting, created
    int priority; // different priorities: high, normal, low
    int pid;
    uint32_t *sp[256]; 
};

TStatus RVCInitalize(uint32_t *gp) {
    //void* s_array[sizeof(struct TCB)];
    struct TCB* mainThread = (struct TCB*)malloc(sizeof(struct TCB)); // initializing TCB of main thread
    //struct TCB* mainThread = s_array;
    mainThread->tid = 0;
    mainThread->state = RVCOS_THREAD_STATE_CREATED;
    mainThread->priority = RVCOS_THREAD_PRIORITY_NORMAL;
    mainThread->pid = -1; // main thread has no parent so set to -1
    threadArray[0] = mainThread; 
    // for gp and sp need to call an assembly function
    
    struct TCB* idleThread = (struct TCB*)malloc(sizeof(struct TCB)); // initializing TCB of idle thread
    idleThread->tid = 1;
    idleThread->state = RVCOS_THREAD_STATE_CREATED;
    idleThread->priority = RVCOS_THREAD_PRIORITY_LOW;
    idleThread->pid = -1;
    threadArray[1] = idleThread;
    // call assembly function to set gp and sp

    global_p = *gp; 
    if (global_p == 0) {
    // Failure since it didn't change global variable
        return RVCOS_STATUS_FAILURE;
    } else {
    // return success
        return RVCOS_STATUS_SUCCESS;
    }
}

TStatus RVCWriteText(const TTextCharacter *buffer, TMemorySize writesize){
    if (buffer == NULL){
        return RVCOS_STATUS_ERROR_INVALID_PARAMETER; 
    }
    else{
        //write out writesize characters to the location specified by buffer
        for (int i = 0; i < writesize; i++) {
            char c = *buffer;
            VIDEO_MEMORY[global + i] = c;
            buffer++;
        }
        global = global + writesize;
        // if there are errors try casting (int)
    }
}

int main() {
    saved_sp = &controller_status;
    while(1){
        if(CART_STAT_REG & 0x1){
            enter_cartridge();
        }
    }
    return 0;
}

uint32_t c_syscall_handler(uint32_t p1,uint32_t p2,uint32_t p3,uint32_t p4,uint32_t p5,uint32_t code){
    return code + 1;
}
