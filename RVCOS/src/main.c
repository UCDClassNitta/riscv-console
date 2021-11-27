#include <stdint.h>
#include <stdlib.h>
#include "RVCOS.h"

volatile int global; // used for global cursor
volatile uint32_t controller_status = 0;
volatile uint32_t *saved_sp;
volatile uint32_t app_global_p;
#define MTIMECMP_LOW    (*((volatile uint32_t *)0x40000010))
#define MTIMECMP_HIGH   (*((volatile uint32_t *)0x40000014))
typedef void (*TFunctionPointer)(void);
void enter_cartridge(void);
uint32_t call_th_ent(void *param, TThreadEntry entry, uint32_t *gp);
void ContextSwitch(volatile uint32_t **oldsp, volatile uint32_t *newsp);
#define CART_STAT_REG (*(volatile uint32_t *)0x4000001C)
#define CONTROLLER_STATUS_REG (*(volatile uint32_t*)0x40000018) // base address of the Multi-Button Controller Status Register
volatile char *VIDEO_MEMORY = (volatile char *)(0x50000000 + 0xFE800);  // taken from riscv-example, main code
volatile struct TCB* threadArray[256]; 
volatile int global_tid_nums = 1;  // should only be 2-256

struct TCB{
    TThreadID tid;
    uint32_t *gp;
    TThreadState state; // different states: running, ready, dead, waiting, created
    TThreadPriority priority; // different priorities: high, normal, low
    //int pid;
    uint32_t *sp; 
    TMemorySize memsize;
    TThreadEntry entry;
    void *param;
    int ticks;
    int wait_id;
    TStatus ret_val;
    uint8_t* stack_base; // return value of malloc
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

uint32_t *init_Stack(uint32_t* sp, TThreadEntry function, uint32_t param, uint32_t tp){
    sp--;
    *sp = (uint32_t)function; //sw      ra,48(sp)
    sp--;
    *sp = tp;//sw      tp,44(sp)  this will hold the current thread's id
    sp--;
    *sp = 0;//sw      t0,40(sp)
    sp--;
    *sp = 0;//sw      t1,36(sp)
    sp--;
    *sp = 0;//sw      t2,32(sp)
    sp--;
    *sp = 0;//sw      s0,28(sp)
    sp--;
    *sp = 0;//sw      s1,24(sp)
    sp--;
    *sp = param;//sw      a0,20(sp)
    sp--;
    *sp = 0;//sw      a1,16(sp)
    sp--;
    *sp = 0;//sw      a2,12(sp)
    sp--;
    *sp = 0;//sw      a3,8(sp)
    sp--;
    *sp = 0;//sw      a4,4(sp)
    sp--;
    *sp = 0;//sw      a5,0(sp)
    return sp;
}

TThreadEntry idle(){
    while(1);
}

void* skeleton(TThreadID thread_id){
    struct TCB* currThread = threadArray[thread_id]; 
    TThreadEntry entry = currThread->entry;
    void* param = currThread->param;
    asm volatile ("csrw mie, %0" : : "r"(0x888));   // Enable all interrupt soruces: csr_write_mie(0x888); 
    asm volatile ("csrsi mstatus, 0x8");            // Global interrupt enable: csr_enable_interrupts()   
    MTIMECMP_LOW = 1;
    MTIMECMP_HIGH = 0;
    // call entry(param) but make sure to switch the gp right before the call

    TStatus ret_val = call_th_ent(entry, param, app_global_p); 
    // Disable intterupts before terminate
    //Threadterminate;
    
}

TStatus RVCInitialize(uint32_t *gp) {
<<<<<<< Updated upstream
    struct TCB* mainThread = (struct TCB*)malloc(sizeof(struct TCB)); // initializing TCB of main thread
=======
    FSSAllocatorInit(&FreeChunkAllocator, sizeof(FreeChunk));

    for(int Index = 0; Index < 8; Index++){
    DeallocateFreeChunk(&InitialFreeChunks[Index]);
    }
    RVCMemoryPoolAllocate(0, threadArraySize * sizeof(void *), (void**)&threadArray);
    for (int i = 0; i < threadArraySize; i++) {
        threadArray[i] = NULL;
    }
    scheduleQ = createQueue(256); // grow if we hit limit
    waiterQ = createReadyQ(256);
    sleeperQ = createReadyQ(256);
    writerQ = createReadyQ(256);
    RVCMemoryPoolAllocate(0, 256 * sizeof(void*), (void**)&mutexArray);
    for (int i = 0; i < 256; i++) {
        mutexArray[i] = NULL;
    }
    RVCMemoryPoolAllocate(0, 256 * sizeof(void*), (void**)&memPoolArray);
    for (int i = 0; i < 256; i++) {
        memPoolArray[i] = NULL;
    }
    struct TCB* mainThread;  // initializing TCB of main thread
    RVCMemoryPoolAllocate(0, sizeof(struct TCB), (void**)&mainThread);
    FSSAllocatorInit(&MPCBAllocator, sizeof(struct MPCB));
    FSSAllocatorInit(&TCBAllocator, sizeof(struct TCB));
    FSSAllocatorInit(&MxAllocator, sizeof(struct Mutex));
    
    // FSSAllocator TCBPool;
    // FSSAllocatorInit(&TCBPool, sizeof(struct TCB));
    
>>>>>>> Stashed changes
    mainThread->tid = 0;
    mainThread->state = RVCOS_THREAD_STATE_CREATED;
    mainThread->priority = RVCOS_THREAD_PRIORITY_NORMAL;
    //mainThread->pid = -1; // main thread has no parent so set to -1
    threadArray[0] = mainThread;
    
    struct TCB* idleThread = (struct TCB*)malloc(sizeof(struct TCB)); // initializing TCB of idle thread
    idleThread->tid = 1;
    idleThread->state = RVCOS_THREAD_STATE_READY;   // idle thread needs to be in ready state
    idleThread->priority = RVCOS_THREAD_PRIORITY_LOWEST;
    //idleThread->pid = -1;
    threadArray[1] = idleThread;
    idleThread->entry = idle();
    idleThread->stack_base = malloc(1024);
    idleThread->sp = init_Stack((uint32_t*)(idleThread->stack_base + 1024), idle(), idleThread->param, idleThread->tid);

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
        if (global < 2304) { // 2304 = 64 columns x 36 rows
        
            for (int i = 0; i < (int)writesize; i++) {
                char c = buffer[i];
<<<<<<< Updated upstream
                VIDEO_MEMORY[global] = ' ';
                if (c == '\n') {
                    global += 0x40;
                    global -= 1;
                    VIDEO_MEMORY[global] = c;
=======
                if (c == '\x1B') {
                    i++;
                    if (i > (int)writesize) {
                        continue;
                    }
                    char c = buffer[i];
                    if (c == '[') {
                        i++;
                        if (i > (int)writesize) {
                            continue;
                        }
                        char c = buffer[i];
                        if (c == 'A') {
                            if (cursor >= 0x40) {
                                cursor -= 0x40;
                            }
                        } else if (c == 'B') {
                            cursor += 0x40;
                            if (cursor >= 2304) {
                                memmove(VIDEO_MEMORY, VIDEO_MEMORY + 0x40, 64*35);
                                memset(VIDEO_MEMORY + 64*35, 0, 64);
                                cursor -= 0x40;
                            }
                        } else if (c == 'C') {
                            if (cursor % 0x40 != 63) { // only move right if not at the right of screen
                                cursor += 1;
                                continue;
                            }
                        } else if (c == 'D') {
                            //if (cursor % 0x40 != 0) { // only move left if not at left of screen
                            cursor -= 1;
                            //}
                        } else if (c == 'H') {
                            cursor = 0;
                        } else if (isdigit(c)){
                            int skip = 0;
                            int ln = (int)c - '0';
                            if (c == '2') {
                                skip = 1;
                                i++;
                                c = buffer[i];
                                if (c == 'J') {
                                    // Erase screen (zero out video_memory)
                                    memset(VIDEO_MEMORY, 0, 2304);
                                    continue;
                                } else {
                                    //VIDEO_MEMORY[50] = 'A';
                                }
                            }
                            //VIDEO_MEMORY[52] = '4';
                            if (!skip) {
                                i++;
                                c = buffer[i];
                            }
                            // 2 digit line number
                            if (isdigit(c)) {
                                //VIDEO_MEMORY[52] = c;
                                ln = ln * 10 + (int)c - '0';
                                i++;
                                if (i > (int)writesize) {
                                    break;
                                }
                                c = buffer[i];
                            }
                            // problem if H or ln is more than 1 char
                            if (c == ';') {
                                //VIDEO_MEMORY[54] = 'B';
                                i++;
                                if (i > (int)writesize) {
                                    continue;
                                }
                                c = buffer[i];
                                int col = (int)c - '0';
                                i++;
                                if (i > (int)writesize) {
                                    continue;
                                }
                                c = buffer[i];
                                // 2 digit column number
                                if (isdigit(c)) {
                                    col = col * 10 + (int)c - '0';
                                    i++;
                                    c = buffer[i];
                                }
                                if (c == 'H') {
                                    cursor = (64 * ln) + col;
                                }
                            }
                        }
                    }
                }
                else if (c == '\n') {
                    cursor += 0x40;
                    cursor = cursor & ~0x3F;
                    if (cursor >= 2304) {
                        memmove(VIDEO_MEMORY, VIDEO_MEMORY + 0x40, 64*35);
                        memset(VIDEO_MEMORY + 64*35, 0, 64);
                        cursor -= 0x40;
                    }
>>>>>>> Stashed changes
                } else if(c == '\b') {
                    global -= 1;
                    VIDEO_MEMORY[global] = c;
                } else {
                    VIDEO_MEMORY[global] = c;
                    global++;
                }
            }
            //VIDEO_MEMORY[global] = *buffer;
            //global = global + writesize;
        }
        // if there are errors try casting (int)
        return RVCOS_STATUS_SUCCESS;
    }
}
 
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
        global_tid_nums++;  // starts at 2, since global_tid_nums is initialized to 2
        if(global_tid_nums > 256){
            return RVCOS_STATUS_ERROR_INSUFFICIENT_RESOURCES;
        }
        struct TCB* newThread = (struct TCB*)malloc(sizeof(struct TCB)); // initializing TCB of a thread
        newThread->stack_base = malloc(memsize); // initialize stack of memsize for the newThread
        newThread->entry = entry;
        newThread->param = param;
        newThread->memsize = memsize;
        newThread->tid = global_tid_nums; 
        tid = newThread->tid;
        newThread->state = RVCOS_THREAD_STATE_CREATED;
        newThread->priority = prio;
        //newThread->pid = -1; 
        threadArray[global_tid_nums] = newThread;
    }
    return RVCOS_STATUS_SUCCESS;
}

TStatus RVCThreadDelete(TThreadID thread){
    if (threadArray[thread] == NULL){
        return RVCOS_STATUS_ERROR_INVALID_ID;
    }
    struct TCB* currThread = threadArray[thread];
    if (currThread->state != RVCOS_THREAD_STATE_DEAD){
        return  RVCOS_STATUS_ERROR_INVALID_STATE;
    }
    else{
        threadArray[thread] = NULL;
        free(currThread->stack_base);
        free(currThread);
        return RVCOS_STATUS_SUCCESS; 
    }
}

TStatus RVCThreadActivate(TThreadID thread){   // we handle scheduling and context switching in here
    if (threadArray[thread] == NULL){
        return RVCOS_STATUS_ERROR_INVALID_ID;
    }
    struct TCB* currThread = threadArray[thread];
    if (currThread->state != RVCOS_THREAD_STATE_DEAD || currThread->state != RVCOS_THREAD_STATE_CREATED){
        return  RVCOS_STATUS_ERROR_INVALID_STATE;
    }
<<<<<<< Updated upstream
    else{
        currThread->sp = init_Stack((uint32_t*)(currThread->stack_base + currThread->memsize), skeleton(currThread->tid), currThread->param, currThread->tid); // initializes stack/ activates thread
        // need to make queues next
        currThread->state = RVCOS_THREAD_STATE_READY;
        return RVCOS_STATUS_SUCCESS; 
=======
    currThread->state = RVCOS_THREAD_STATE_DEAD;
    currThread->ret_val = returnval;

    if(waiterQ->size != 0){
        int flag = 0;
        for(int i = 0; i < waiterQ->size; i++){
            int waiterTID = removeRQ(waiterQ);
            struct TCB* waiter = threadArray[waiterTID];
            // if the thread terminating is the thread that a waiter is waiting on
            if (waiter->wait_id == thread){
                waiter->ret_val = returnval;
                waiter->state = RVCOS_THREAD_STATE_READY;
                enqueueThread(waiter);
                if(waiter->priority > currThread->priority){
                    flag = 1;
                }
            }
            else{
                insertRQ(waiterQ,waiter->tid);
            }
        }
        if(flag == 1){
            schedule();
        }
    }
    //If the thread terminating is the current running thread, then you will definitely need to schedule.
    if(threadArray[get_tp()] == currThread){
        schedule();
    }
    return RVCOS_STATUS_SUCCESS;
}

TStatus RVCThreadWait(TThreadID thread, TThreadReturnRef returnref, TTick timeout) {
    if (threadArray[thread] == NULL){
        return RVCOS_STATUS_ERROR_INVALID_ID;
    }
    else if(returnref == NULL){
        return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
    }
    struct TCB* currThread = threadArray[get_tp()];
    struct TCB* waitThread = threadArray[thread];
    
    if (timeout == RVCOS_TIMEOUT_IMMEDIATE){
        //the current returns immediately regardless if the thread has terminated
        if(waitThread->state != RVCOS_THREAD_STATE_DEAD){
            return RVCOS_STATUS_FAILURE;
        }
        else{
            *returnref = (TThreadReturn)currThread->ret_val;
            return RVCOS_STATUS_SUCCESS;
        }
    }
    else if (timeout == RVCOS_TIMEOUT_INFINITE){
        if (waitThread->state != RVCOS_THREAD_STATE_DEAD) {
            currThread->state = RVCOS_THREAD_STATE_WAITING;
            currThread->wait_id = thread;
            insertRQ(waiterQ,currThread->tid);
            schedule();
            *returnref = (TThreadReturn)currThread->ret_val;
            return RVCOS_STATUS_SUCCESS;
        }
        else {
            *returnref = (TThreadReturn)waitThread->ret_val; // not sure what this is for
            return RVCOS_STATUS_SUCCESS;
        }
        
    }
    else{
        currThread->ticks = timeout;
        currThread->state = RVCOS_THREAD_STATE_WAITING;
        insertRQ(sleeperQ, currThread->tid);
        numSleepers++;
        schedule();
        // schedules next thread after putting current thread to sleep
        // checks the timeout of the current thread that was put to sleep
        // if the timeout expired and the thread it was waiting on is not dead yet, return failure
        if(currThread->ticks == 0 && waitThread->state != RVCOS_THREAD_STATE_DEAD){
            return RVCOS_STATUS_FAILURE;
        }
        if (waitThread->state != RVCOS_THREAD_STATE_DEAD) {
            currThread->state = RVCOS_THREAD_STATE_WAITING;
            currThread->wait_id = thread;
            insertRQ(waiterQ,currThread->tid);
            schedule();
            *returnref = (TThreadReturn)currThread->ret_val;
            return RVCOS_STATUS_SUCCESS;
        }
        //*returnref = (TThreadReturn)waitThread->ret_val;   
        return RVCOS_STATUS_SUCCESS;
    }
}

TStatus RVCThreadID(TThreadIDRef threadref){
    if (threadref == NULL){
        return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
    }
    *threadref = get_tp();
    return RVCOS_STATUS_SUCCESS;
}

TStatus RVCThreadState(TThreadID thread, TThreadStateRef state){
    if (state == NULL){
        return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
    }
    else if (threadArray[thread] == NULL){
        return RVCOS_STATUS_ERROR_INVALID_ID;
    }
    *state = threadArray[thread]->state;
    return RVCOS_STATUS_SUCCESS;
}

void schedule(){
    struct TCB* current = threadArray[get_tp()];
    int nextTid = 0;
    struct TCB* nextT;
    if (scheduleQ->highSize != 0) {

        nextTid = removeData(scheduleQ, RVCOS_THREAD_PRIORITY_HIGH);

    } else if (scheduleQ->norSize != 0) {

        nextTid = removeData(scheduleQ, RVCOS_THREAD_PRIORITY_NORMAL);

    } else if (scheduleQ->lowSize != 0) {
        nextTid = removeData(scheduleQ, RVCOS_THREAD_PRIORITY_LOW);
    } else {
        nextTid = 1;
    }
    nextT = threadArray[nextTid];
    if(current->tid != nextTid){
        nextT->state = RVCOS_THREAD_STATE_RUNNING;
        
        ContextSwitch((void *)&current->sp, threadArray[nextTid]->sp);
        
    }
    else{
        current->state = RVCOS_THREAD_STATE_RUNNING;
    }

}

TStatus RVCThreadSleep(TTick tick) {
    if (tick == RVCOS_TIMEOUT_INFINITE){
        return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
    }
    else if(tick == RVCOS_TIMEOUT_IMMEDIATE){
        struct TCB* current = threadArray[get_tp()];
        TThreadPriority currPrio = current->priority;
        
        int next = removeData(scheduleQ, currPrio);
        if (next == 1) {
            enqueueThread(current);
        }
        struct TCB* nextRT = threadArray[next];
        enqueueThread(nextRT);
    } else {
        struct TCB* current = threadArray[get_tp()];
        current->ticks = tick;
        current->state = RVCOS_THREAD_STATE_WAITING;
        insertRQ(sleeperQ, current->tid);
        //sleepers[sleeperCursor] = current;
        //sleeperCursor++;
        numSleepers++;
        schedule();
        return RVCOS_STATUS_SUCCESS;
    }
}

TStatus RVCTickMS(uint32_t *tickmsref) {
    if (tickmsref == NULL) {
        return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
    }
    *tickmsref = 2; // 2000/MTIME;
    return RVCOS_STATUS_SUCCESS;
}

TStatus RVCTickCount(TTickRef tickref) {
    if (tickref == NULL){
        return RVCOS_STATUS_ERROR_INVALID_PARAMETER ;
    }
    else{
        *tickref = tick_count;
        return RVCOS_STATUS_SUCCESS;
    }
}


TStatus  RVCMemoryPoolCreate(void  *base,  TMemorySize  size,  TMemoryPoolIDRef memoryref) {
    if (base == NULL || size < 128) {
        return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
    }
    struct MPCB *memPool;
    memPool = AllocateMPCB();
    memPool->base = base;
    memPool->size = size;
    memPool->freeSize = size;

    memPool->firstFree = AllocateFreeChunk();
    memPool->firstFree->size = size;
    memPool->firstFree->base = base;
    memPool->firstFree->next = NULL;
    memPool->allocList = NULL;
    memPool->mpid = global_mpid_nums;
    memPoolArray[global_mpid_nums] = memPool;
    *memoryref = global_mpid_nums;
    global_mpid_nums++;
    return RVCOS_STATUS_SUCCESS;
}

TStatus RVCMemoryPoolDelete(TMemoryPoolID memory) {

    if (memory == RVCOS_MEMORY_POOL_ID_SYSTEM) { // Or if memory is invalid memory pool
        return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
    }
    // if any memory has been allocated from the pool and is not deallocated
    //   return RVCOS_STATUS_ERROR_INVALID_STATE 
}

TStatus RVCMemoryPoolQuery(TMemoryPoolID memory, TMemorySizeRef bytesleft) {
    if (memory == -1) { // if memory is invalid memory pool
        return RVCOS_STATUS_ERROR_INVALID_ID;
    }
    if (bytesleft == NULL) { // Or if memory is invalid memory pool
        return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
    }
    *bytesleft = memPoolArray[memory]->freeSize;
    return RVCOS_STATUS_SUCCESS;
}

// allocate might call allocatefreechunk
TStatus RVCMemoryPoolAllocate(TMemoryPoolID memory, TMemorySize size, void **pointer) {
    if (size == 0) { // Or if memory is invalid memory pool
        //RVCWriteText1("1\n", 2);
        return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
    }
    else if ( pointer == NULL ) {
        //RVCWriteText1("2\n", 2);
        return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
    }
    else if (memory == RVCOS_MEMORY_POOL_ID_SYSTEM){ //Check if this size is 0 this causes error
        // if(SystemPoolSize < size){
        //     return RVCOS_STATUS_ERROR_INSUFFICIENT_RESOURCES;
        // }
        //RVCWriteText1("3\n", 2);
        *pointer = (void *)malloc(size);
        return RVCOS_STATUS_SUCCESS;
    }
    else if (memPoolArray[memory] == NULL || memory == -1 ){
        //RVCWriteText1("4\n", 2);
        return RVCOS_STATUS_ERROR_INVALID_ID;
    }
    else if(memPoolArray[memory]->freeSize < size){
        //RVCWriteText1("5\n", 2);
        return RVCOS_STATUS_ERROR_INSUFFICIENT_RESOURCES;
    }
    else{
        //RVCWriteText1("6\n", 2);
        struct MPCB *currPool = memPoolArray[memory];
        uint32_t alloc_size = ((size + 63)/64) * 64;
        FreeChunkRef cur = currPool->firstFree;
        FreeChunkRef prev = NULL;
        while(cur) {
            if (alloc_size <= cur->size) {
                
                if (alloc_size == cur->size) {
                    if (prev) {
                        prev->next = cur->next;
                    } else {
                        currPool->firstFree = cur->next;
                    }
                    FreeChunkRef tmp = currPool->allocList; // add newnode to alloc
                    cur -> next = tmp;                      // move cur
                    currPool -> allocList = cur;             // alloc list
                    *pointer = cur->base;               // return ptr cur->base
                    return RVCOS_STATUS_SUCCESS;

                }
                else {
                    FreeChunk *newnode = MemoryAlloc(alloc_size);
                    newnode->base = cur->base;
                    newnode->size = alloc_size;
                    cur->base += alloc_size;
                    cur->size -= alloc_size;

                    FreeChunkRef tmp = currPool->allocList; // add newnode to alloc
                    newnode -> next = tmp;
                    currPool -> allocList = newnode;
                    *pointer = newnode->base; // return newnode->base
                    return RVCOS_STATUS_SUCCESS;
                }
            }
            prev = cur;
            cur = cur->next;
        }
        return RVCOS_STATUS_ERROR_INSUFFICIENT_RESOURCES;
    }
}




TStatus RVCMemoryPoolDeallocate(TMemoryPoolID memory, void *pointer) {
    if (pointer == NULL) { // Or if memory is invalid memory pool
        return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
    }
    if (memory == -1 || memPoolArray[memory] == NULL) {
        return RVCOS_STATUS_ERROR_INVALID_ID;
    }
    if (memory == 0) {
        free(pointer);
        return RVCOS_STATUS_SUCCESS;
    }
    struct MPCB* mPool = memPoolArray[memory];
    FreeChunkRef curAlloc = mPool->allocList;
    FreeChunkRef prevAlloc = NULL; 
    FreeChunkRef curFree = mPool->firstFree;
    FreeChunkRef prevFree = NULL; 
    FreeChunkRef newFree = NULL; 
    while (curAlloc) {
        if (curAlloc->base == pointer) {
            // found in alloclist
            if (prevAlloc) {
                prevAlloc->next = curAlloc->next;
            }
            else {
                mPool->allocList = curAlloc->next;
            }
             // remove node from alloclist
            newFree = curAlloc;
            break;
        }
        prevAlloc = curAlloc;
        curAlloc = curAlloc->next;
    }

    while(curFree) {
        if (newFree->base < curFree->base) {
            newFree->next = curFree;
            if (prevFree) {
                prevFree->next = newFree;
            } else {
                mPool->firstFree = newFree;
            }
            break;
        }
        prevFree = curFree;
        curFree = curFree->next;
>>>>>>> Stashed changes
    }
}

<<<<<<< Updated upstream
=======
TStatus RVCMutexCreate(TMutexIDRef mutexref) {
    if (mutexref == NULL) {
         return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
    }
    struct Mutex *mx;
    RVCMemoryPoolAllocate(0, sizeof(struct Mutex), (void**)&mx);
    struct PrioQ *mxQueue = createQueue(256);

    mx -> pq = mxQueue;
    mx -> mxid = num_mutex;
    mx -> unlocked = 1;
    mx -> holder = NULL;
    mutexArray[num_mutex] = mx;
    *mutexref = num_mutex;
    num_mutex ++;
    return RVCOS_STATUS_SUCCESS;
}

TStatus RVCMutexDelete(TMutexID mutex) {
    if(mutexArray[mutex] == NULL){
        return RVCOS_STATUS_ERROR_INVALID_ID;
    }
    else if(mutexArray[mutex]->unlocked != 1){
        return RVCOS_STATUS_ERROR_INVALID_STATE;
    }
    else{
        RVCMemoryPoolDeallocate(0, mutexArray[mutex]);
        mutexArray[mutex] = NULL;
        num_mutex--;
        return RVCOS_STATUS_SUCCESS;   
    }
}

TStatus RVCMutexQuery(TMutexID mutex, TThreadIDRef ownerref) {
    if (ownerref == NULL) {
        return RVCOS_STATUS_ERROR_INVALID_PARAMETER; 
    }
    // If mutex is unlocked
    if (mutexArray[mutex]->unlocked) {
        *ownerref = RVCOS_THREAD_ID_INVALID;
        return RVCOS_THREAD_ID_INVALID;
    }
    // If mutex doesn't exist
    if (mutexArray[mutex] == NULL) {
        return RVCOS_STATUS_ERROR_INVALID_ID;
    }
    *ownerref = mutexArray[mutex] -> holder;
    return RVCOS_STATUS_SUCCESS;
}

TStatus RVCMutexAcquire(TMutexID mutex, TTick timeout) {
    if (mutexArray[mutex] == NULL) {
        return RVCOS_STATUS_ERROR_INVALID_ID;
    }
    struct Mutex *mx = mutexArray[mutex];
    struct TCB *currThread = threadArray[get_tp()];
    // If timeout specified as IMMEDIATE, current returns immediately if mutex is locked 
    if (timeout == RVCOS_TIMEOUT_IMMEDIATE) {
        if (mx->unlocked != 1) {
            return RVCOS_STATUS_FAILURE;
        }
    }
    // If timeout is specified as INFINITE, thread will block until mutex is acquired. 
    else if (timeout == RVCOS_TIMEOUT_INFINITE) {
        // check if mutex is unlocked
        if (mx->unlocked == 1){
            // mutex is now held by the current running thread
            mx->holder = currThread->tid;
            // lock mutex
            mx->unlocked = 0;
        }else{
            // mutex is locked and the thread will block until mutex is unlocked
            currThread->state = RVCOS_THREAD_STATE_WAITING;
            // set currthread to waiting, add thread to pq, 
            insert(mx->pq, currThread->tid, currThread->priority);
            // need to schedule the next thread cause this one is waiting for the mutex to be released
            schedule();
        }
    }
    // If  the  timeout  expires  prior  to  the  acquisition  of  the  mutex, RVCOS_STATUS_FAILURE  is  returned.  
    else{
        // how do we check if the timeout expires, need to ask in OH about this
        if (mx->unlocked == 1){
            // mutex is now held by the current running thread
            mx->holder = currThread->tid;
            // lock mutex
            mx->unlocked = 0;
        }else{
            // mutex is locked and the thread will block until mutex is unlocked
            currThread->state = RVCOS_THREAD_STATE_WAITING;
            currThread->ticks = timeout;
            insertRQ(sleeperQ, currThread->tid);
            if(currThread->ticks == 0){
                 return RVCOS_STATUS_FAILURE;
                
            }
            // set currthread to waiting, add thread to pq, 
            //insert(mx->pq, currThread->tid, currThread->priority);
            // need to schedule the next thread cause this one is waiting for the mutex to be released

            schedule();
        }
        return RVCOS_STATUS_FAILURE;
    }
    return RVCOS_STATUS_SUCCESS;
}

//RVCMutexRelease() releases the mutex specified by the mutex parameter that is currently held by 
// the running thread. Release of the mutex may cause another higher priority thread to be scheduled 
// if it acquires the newly released mutex.  
TStatus RVCMutexRelease(TMutexID mutex) {
    if (mutexArray[mutex] == NULL) {
        return RVCOS_STATUS_ERROR_INVALID_ID;
    }
    else if(mutexArray[mutex]->holder != get_tp()){ 
        return RVCOS_STATUS_ERROR_INVALID_STATE;
    }
    else{
        struct Mutex *mx = mutexArray[mutex];
        mx->holder = NULL;
        mx->unlocked = 1;
        int nextTid = removeData(mx->pq, RVCOS_THREAD_PRIORITY_HIGH);
        if(nextTid == -1){
            nextTid = removeData(mx->pq, RVCOS_THREAD_PRIORITY_NORMAL);
            if(nextTid == -1){
               nextTid = removeData(mx->pq, RVCOS_THREAD_PRIORITY_LOW);
            }
        }
        // nothing in any of the pqs
        if(nextTid == -1){
            return RVCOS_STATUS_SUCCESS;
        }
        else{
            struct TCB *nextThread = threadArray[nextTid];
            nextThread->state = RVCOS_THREAD_STATE_READY;
            enqueueThread(nextThread);
            mx->holder = nextTid;
            mx->unlocked = 0;
            //schedule();
            if (nextThread->priority > threadArray[get_tp()]->priority){
                threadArray[get_tp()]->state = RVCOS_THREAD_STATE_READY;
                enqueueThread(threadArray[get_tp()]);
                schedule();
            }
            
            return RVCOS_STATUS_SUCCESS;
        }
    }
}

TStatus RVCChangeVideoMode(TVideoMode mode){
    if (mode != RVCOS_VIDEO_MODE_TEXT || mode != RVCOS_VIDEO_MODE_GRAPHICS){
        return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
    }
    ModeControl->DMode = mode; // changes the mode, and then blocks til next video refresh
    struct TCB* currentThread = threadArray[get_tp()];
    currentThread->buffer = NULL;
    currentThread->writesize = 0; // this will signal that its a video mode change instead of blocking for a write
    currentThread->state = RVCOS_THREAD_STATE_WAITING;
    insertRQ(writerQ, currentThread->tid);
    // Insert current before scheduling
    schedule();
    return RVCOS_STATUS_SUCCESS;
}

TStatus RVCGraphicCreate(TGraphicType type, TGraphicIDRef gidref){
    if(type != RVCOS_GRAPHIC_TYPE_FULL || type != RVCOS_GRAPHIC_TYPE_LARGE || type != RVCOS_GRAPHIC_TYPE_SMALL){
        return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
    }
    else if(gidref == NULL){
        return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
    }
    else{
        struct GCB* newGraphic = AllocateGCB();
        if(type == RVCOS_GRAPHIC_TYPE_FULL){
            RVCMemoryAllocate(512*288,newGraphic->buffer);
            newGraphic->height = 512;
            newGraphic->width = 288;
        }
        else if(type == RVCOS_GRAPHIC_TYPE_LARGE){
            RVCMemoryAllocate(64*64,newGraphic->buffer);
            newGraphic->height = 64;
            newGraphic->width = 64;
        }
        else{
            RVCMemoryAllocate(16*16,newGraphic->buffer);
            newGraphic->height = 16;
            newGraphic->width = 16;
        }
        newGraphic->type = type;
        newGraphic->gid = global_gid_nums;
        newGraphic->state = RVCOS_GRAPHIC_STATE_DEACTIVATED;  
        offscreenBufferArray[global_gid_nums] = newGraphic;
        *gidref = global_gid_nums;
        global_gid_nums++;
        return RVCOS_STATUS_SUCCESS;
    }
}

TStatus RVCGraphicDelete(TGraphicID gid){
    return RVCOS_STATUS_SUCCESS;
}

TStatus RVCGraphicActivate(TGraphicID gid, SGraphicPositionRef pos, SGraphicDimensionsRef dim, TPaletteID pid){
  /*  Upon successful activation of the background buffer, RVCGraphicActivate() returns 
    RVCOS_STATUS_SUCCESS. If the graphic buffer identifier gid does not exist or if the palette 
    identifier pid does not exist, then RVCOS_STATUS_ERROR_INVALID_ID is returned. If pos is 
    NULL, dim is NULL for a non-FULL graphic, or if pos or dim non-NULL but are out of range for 
    the  type  of  graphic  then  RVCOS_STATUS_ERROR_INVALID_PARAMETER  is  returned.  If 
    there are insufficient video hardware resources to activate the graphic buffer, then 
    RVCOS_STATUS_ERROR_INSUFFICIENT_RESOURCES  is  returned.  If  the  graphic  buffer 
    has a pending activation, then RVCOS_STATUS_ERROR_INVALID_STATE is returned*/
    return RVCOS_STATUS_SUCCESS;
}

TStatus RVCGraphicDeactivate(TGraphicID gid){
    return RVCOS_STATUS_SUCCESS;
}

int* determineOverlap(struct GCB* graphic, SGraphicPositionRef pos, SGraphicDimensionsRef dim, uint32_t srcwidth, int graphicSize, int **bufPosArray){
    int *overlap;
    int *buf;
    RVCMemoryPoolAllocate(0, graphicSize * sizeof(int), (void**)&overlap); //147456 = max pixels in graphic
    RVCMemoryPoolAllocate(0, graphicSize * sizeof(int), (void**)&buf);
    int topLeft = (pos->DYPosition * graphic->width) + pos->DXPosition;
    int srcPos = 0;
    int graphPos = 0;
    int row = 0; // row in source
    int col = 0; // col in source
    int i = 0; // index of overlap array
    while(1) {
        if (pos > (dim->DHeight-1 * srcwidth) + dim->DWidth-1) {
            break;
        }
        if (col <= dim->DWidth - 1 && row <= dim->DHeight - 1) {
            overlap[i] = pos;
            buf[i] = graphPos + topLeft;
            i++;
        }
        if (col == srcwidth - 1) {
            col = 0;
            row++;
            graphPos += graphic->width - srcwidth - 1;
        } else {
            col++;
        }
        pos++;
    }
    *bufPosArray = buf;
    RVCMemoryPoolDeallocate(0, buf);
    return overlap;
}


TStatus RVCGraphicDraw(TGraphicID gid, SGraphicPositionRef pos, SGraphicDimensionsRef dim, TPaletteIndex *src, uint32_t srcwidth){
    /*Upon successful activation of the background buffer, RVCGraphicDraw() returns 
    RVCOS_STATUS_SUCCESS. If the graphic buffer identifier gid does not exist, then 
    RVCOS_STATUS_ERROR_INVALID_ID  is  returned.  If  pos  is  NULL,  dim  is  NULL,  src  is 
    NULL or srcwidth is less than DWidth in dim, then 
    RVCOS_STATUS_ERROR_INVALID_PARAMETER is returned. If the buffer has been 
    activated, but the activation has not completed (the upcall has not been invoked), then 
    RVCOS_STATUS_ERROR_INVALID_STATE is returned. */
    /*struct GCB* graphic = offscreenBufferArray[gid];
    graphic->buffer = graphic->buffer + pos->DYPosition*dim->DWidth +pos->DXPosition;
    for(int i = 0; i< row_count; i++){
        memcpy(graphic->buffer, src, col_count);
        graphic->buffer += dim->DWidth;
        src += srcwidth;
    }*/
    struct GCB* graphic = offscreenBufferArray[gid];
    int graphicSize = dim->DWidth * dim->DHeight;
    int *bufPosArray;
    RVCMemoryPoolAllocate(0, graphicSize * sizeof(int), (void**)&bufPosArray);
    int *srcOverlap = determineOverlap(graphic, pos, dim, srcwidth, graphicSize, &bufPosArray);
    for(int i = 0; i< graphicSize; i++){
        int buffPos = bufPosArray[i];
        int srcPos = srcOverlap[i];
        // graphic->buffer = buffer + buffPos
        memcpy((int)graphic->buffer + buffPos, src[srcPos], 1);
    }
    return RVCOS_STATUS_SUCCESS;



    // // ouch ouch my brain hurts. I will come back to this later
    // for (int i = 0; i < graphicSize; i++) {
    //     int srcRow = srcOverlap[i] + 1 / (dim->DWidth) - 1;
    //     int srcCol = srcOverlap[i] % (dim->DWidth) - 1;
    //     bufPosArray[i] = topLeft + srcOverlap[i];

    // }

    return RVCOS_STATUS_SUCCESS;
}

TStatus RVCPaletteCreate(TPaletteIDRef pidref){
    if (pidref == NULL){
        return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
    }
    return RVCOS_STATUS_SUCCESS;
}

TStatus RVCPaletteDelete(TPaletteID pid){
    return RVCOS_STATUS_SUCCESS;
}

TStatus RVCPaletteUpdate(TPaletteID pid, SColorRef cols, TPaletteIndex offset, uint32_t count){
    return RVCOS_STATUS_SUCCESS;
}


>>>>>>> Stashed changes
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
        case 0x00: return RVCInitialize((void *)p1);
        case 0x01: return RVCThreadCreate((void *)p1, p2, p3, p4, p5);
        case 0x02: return RVCThreadDelete((void *)p1);
        case 0x03: return RVCThreadActivate((void *)p1);
        // case 0x04: return RVCThreadTerminate((void *)p1, p2);
        // case 0x05: return RVCThreadWait((void *)p1, p2);
        // case 0x06: return RVCThreadID((void *)p1);
        // case 0x07: return RVCThreadState((void *)p1, p2);
        // case 0x08: return RVCThreadSleep((void *)p1);
        // case 0x09: return RVCTickMS((void *)p1);
        // case 0x0A: return RVCTickCount((void *)p1);
        case 0x0B: return RVCWriteText((void *)p1, p2);
        case 0x0C: return RVCReadController((void *)p1);
    }
    return code + 1;
}
