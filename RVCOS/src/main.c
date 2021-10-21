#include <stdint.h>
#include <stdlib.h>
#include "RVCOS.h"

volatile int global; // used for global cursor
volatile uint32_t controller_status = 0;
volatile uint32_t *saved_sp;
volatile uint32_t app_global_p;
typedef void (*TFunctionPointer)(void);
void enter_cartridge(void);
#define CART_STAT_REG (*(volatile uint32_t *)0x4000001C)
#define CONTROLLER_STATUS_REG (*(volatile uint32_t*)0x40000018) // base address of the Multi-Button Controller Status Register
volatile char *VIDEO_MEMORY = (volatile char *)(0x50000000 + 0xFE800);  // taken from riscv-example, main code

volatile struct TCB* threadArray[256]; 

struct TCB{
    TThreadID tid;
    uint32_t *gp;
    TThreadState state; // different states: running, ready, dead, waiting, created
    TThreadPriority priority; // different priorities: high, normal, low
    int pid;
    uint32_t *sp; 
    TMemorySize memsize;
    TThreadEntry entry;
    void *param;
    int ticks;
    int wait_id;
    TStatus ret_val;
    void* stack_base; // return value of malloc
};

int getTState(struct TCB* thread){
    return thread->state;
}

int getTPrio(struct TCB* thread){
    return thread->priority;
}

int getTID(struct TCB* thread){
    return thread->tid;
}

uint32_t *init_Stack(uint32_t* sp, void (*function)(uint32_t), uint32_t param){
    sp--;
    *sp = (uint32_t)function; // function will either be skeleton or idle
    sp--;
    *sp = param;
    return sp;
}

void idle(){
    while(1);
}

void* skeleton(TThreadID thread_id){
    struct TCB* currThread = threadArray[thread_id]; 
    TThreadEntry entry = currThread->entry;
    void* param = currThread->param;
    csr_write_mie(0x888);       // Enable all interrupt soruces
    csr_enable_interrupts();    // Global interrupt enable
    TStatus ret_val = call_th_ent(entry, param, app_global_p); 
    //Threadterminate;
    
}

TStatus RVCInitialize(uint32_t *gp) {
    struct TCB* mainThread = (struct TCB*)malloc(sizeof(struct TCB)); // initializing TCB of main thread
    mainThread->tid = 0;
    mainThread->state = RVCOS_THREAD_STATE_CREATED;
    mainThread->priority = RVCOS_THREAD_PRIORITY_NORMAL;
    mainThread->pid = -1; // main thread has no parent so set to -1
    threadArray[0] = mainThread;
    
    struct TCB* idleThread = (struct TCB*)malloc(sizeof(struct TCB)); // initializing TCB of idle thread
    idleThread->tid = 1;
    idleThread->state = RVCOS_THREAD_STATE_CREATED;
    idleThread->priority = RVCOS_THREAD_PRIORITY_LOWEST;
    idleThread->pid = -1;
    threadArray[1] = idleThread;

    app_global_p = *gp; 
    if (app_global_p == 0) {
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
        return RVCOS_STATUS_SUCCESS;
    }
}

// typedef struct{ 
//     uint32_t DLeft:1; 
//     uint32_t DUp:1; 
//     uint32_t DDown:1; 
//     uint32_t DRight:1; 
//     uint32_t DButton1:1; 
//     uint32_t DButton2:1; 
//     uint32_t DButton3:1; 
//     uint32_t DButton4:1; 
//     uint32_t DReserved:24; 
// } SControllerStatus, *SControllerStatusRef; 
 
TStatus RVCReadController(SControllerStatusRef statusref){
    if (statusref == NULL){
        return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
    }
    else{
        *(uint32_t *)statusref = CONTROLLER_STATUS_REG; // the struct is 32 bits and aligned with the bits in the register
        return RVCOS_STATUS_SUCCESS;                    // from Piazza post question @410
    }
}

TStatus RVCThreadCreate(TThreadEntry entry, void *param, TMemorySize memsize, 
                        TThreadPriority prio, TThreadIDRef tid){
    if (entry == NULL || tid == NULL){
        return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
    }
    else{
        //void* newThreadStack[memsize];
        struct TCB* newThread;
        newThread->stack_base = (struct TCB*)malloc(memsize); // initializing TCB of a thread
        newThread->entry = entry;
        newThread->param = param;
        newThread->memsize = memsize;
        newThread->tid = 0; // need to come up with way to make tids
        tid = newThread->tid;
        newThread->state = RVCOS_THREAD_STATE_CREATED;
        newThread->priority = prio;
        //newThread->pid = -1; 
        //threadArray[0] = newThread; 
    }
    return RVCOS_STATUS_SUCCESS;
}

int main() {
    saved_sp = &CART_STAT_REG; // was used to see how the compiler would assign the save_sp so we could 
    while(1){                      // do it in assembly in the enter_cartridge function
        if(CART_STAT_REG & 0x1){
            enter_cartridge();
        }
    }
    return 0;
}

uint32_t c_syscall_handler(uint32_t p1,uint32_t p2,uint32_t p3,uint32_t p4,uint32_t p5,uint32_t code){
    switch(code){
        case 0: return RVCInitialize((void *)p1);
        case 0x0B: return RVCWriteText((void *)p1, (void *)p2);
    }
    return code + 1;
}
