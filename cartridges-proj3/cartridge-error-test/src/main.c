#include <stdint.h>
#include <string.h>
#include "RVCOS.h"

void WriteString(const char *str){
    const char *Ptr = str;
    while(*Ptr){
        Ptr++;
    }
    RVCWriteText(str,Ptr-str);
}


TMutexID TheMutex;
volatile int TheMutexAcquired = 0;
volatile int MemoryAllocationSuccess = -1;
volatile char *MemoryBase1;
volatile char *MemoryBase2;

TThreadReturn Thread(void *param){
    char LocalVariable;
    
    WriteString("Thread Alive\nThread testing memory locations\n");
    if((MemoryBase1 < &LocalVariable)&&(&LocalVariable < MemoryBase2)){
        MemoryAllocationSuccess = 1;
    }
    else if((MemoryBase1 > &LocalVariable)&&(&LocalVariable > MemoryBase2)){
        MemoryAllocationSuccess = 1;
    }
    else{
        WriteString("Thread stack not allocated from system memory pool\n");
        MemoryAllocationSuccess = 0;
        return 0;
    }
    WriteString("Thread Acquiring Mutex\n");
    if(RVCOS_STATUS_SUCCESS != RVCMutexAcquire(TheMutex, RVCOS_TIMEOUT_INFINITE)){
        WriteString("Thread failed to acquire mutex\n");
        return 0;
    }
    WriteString("Thread Mutex Acquired\n");
    TheMutexAcquired = 1;
    while(1);
    return 0;
}

int main() {
    TThreadID OtherThreadID, ThisThreadID, BadThreadID, MutexOwner;
    TThreadState RVCState;
    TMutexID BadMutexID;
    TMemoryPoolID MemoryPool1;
    TMemorySize SystemPoolSize;
    TTick CurrentTick, LastTick;
    char *LocalAllocation;
    uint32_t MSPerTick;
    
    
    
    WriteString("Main testing RVCMemoryPoolQuery.\n");
    if(RVCOS_STATUS_ERROR_INVALID_ID != RVCMemoryPoolQuery(RVCOS_MEMORY_POOL_ID_INVALID, &SystemPoolSize)){
        WriteString("MemoryPoolQuery doesn't handle bad memoryid.\n");    
        return 0;
    }
    if(RVCOS_STATUS_ERROR_INVALID_PARAMETER != RVCMemoryPoolQuery(RVCOS_MEMORY_POOL_ID_SYSTEM, NULL)){
        WriteString("MemoryPoolQuery doesn't handle NULL bytesleft.\n");    
        return 0;
    }
    if(RVCOS_STATUS_SUCCESS != RVCMemoryPoolQuery(RVCOS_MEMORY_POOL_ID_SYSTEM, &SystemPoolSize)){
        WriteString("MemoryPoolQuery doesn't return success with valid inputs.\n");    
        return 0;
    }
    WriteString("Main RVCMemoryPoolQuery appears OK.\n");
    WriteString("Main testing RVCMemoryPoolAllocate.\n");
    if(RVCOS_STATUS_ERROR_INVALID_PARAMETER != RVCMemoryPoolAllocate(RVCOS_MEMORY_POOL_ID_SYSTEM, 0, (void **)&MemoryBase1)){
        WriteString("MemoryPoolAllocate doesn't handle zero size.\n");    
        return 0;
    }
    if(RVCOS_STATUS_ERROR_INVALID_PARAMETER != RVCMemoryPoolAllocate(RVCOS_MEMORY_POOL_ID_SYSTEM, 64, NULL)){
        WriteString("MemoryPoolAllocate doesn't handle NULL pointer.\n");    
        return 0;
    }
    if(RVCOS_STATUS_ERROR_INVALID_ID != RVCMemoryPoolAllocate(RVCOS_MEMORY_POOL_ID_INVALID, 64, (void **)&MemoryBase1)){
        WriteString("MemoryPoolAllocate doesn't handle bad memoryid.\n");    
        return 0;
    }
    if(RVCOS_STATUS_ERROR_INSUFFICIENT_RESOURCES  != RVCMemoryPoolAllocate(RVCOS_MEMORY_POOL_ID_SYSTEM, SystemPoolSize + 256, (void **)&MemoryBase1)){
        WriteString("MemoryPoolAllocate doesn't handle insufficient resources.\n");    
        return 0;
    }
    if(RVCOS_STATUS_SUCCESS != RVCMemoryPoolAllocate(RVCOS_MEMORY_POOL_ID_SYSTEM, 64, (void **)&MemoryBase1)){
        WriteString("MemoryPoolAllocate doesn't return success with valid inputs.\n");    
        return 0;
    }
    WriteString("Main RVCMemoryPoolAllocate appears OK.\n");
    WriteString("Main testing RVCThreadCreate.\n");
    if(RVCOS_STATUS_ERROR_INVALID_PARAMETER != RVCThreadCreate(NULL, NULL, 0x1000, RVCOS_THREAD_PRIORITY_NORMAL, &OtherThreadID)){
        WriteString("ThreadCreate doesn't handle NULL entry.\n");    
        return 0;
    }
    if(RVCOS_STATUS_ERROR_INVALID_PARAMETER != RVCThreadCreate(Thread, NULL, 0x1000, RVCOS_THREAD_PRIORITY_NORMAL, NULL)){
        WriteString("ThreadCreate doesn't handle NULL tid.\n");    
        return 0;
    }
    if(RVCOS_STATUS_SUCCESS != RVCThreadCreate(Thread, NULL, 0x1000, RVCOS_THREAD_PRIORITY_NORMAL, &OtherThreadID)){
        WriteString("ThreadCreate doesn't return success with valid inputs.\n");    
        return 0;
    }
    WriteString("Main RVCThreadCreate appears OK.\n");
    if(RVCOS_STATUS_SUCCESS != RVCMemoryPoolAllocate(RVCOS_MEMORY_POOL_ID_SYSTEM, 128, (void **)&MemoryBase2)){
        WriteString("MemoryPoolAllocate doesn't return success with valid inputs.\n");    
        return 0;
    }
    
    WriteString("Main testing RVCMemoryPoolCreate.\n");
    if(RVCOS_STATUS_ERROR_INVALID_PARAMETER != RVCMemoryPoolCreate(NULL, 128, &MemoryPool1)){
        WriteString("MemoryPoolCreate doesn't handle NULL base.\n");    
        return 0;
    }
    if(RVCOS_STATUS_ERROR_INVALID_PARAMETER != RVCMemoryPoolCreate((void *)MemoryBase1, 0, &MemoryPool1)){
        WriteString("MemoryPoolCreate doesn't handle zero size.\n");    
        return 0;
    }
    if(RVCOS_STATUS_ERROR_INVALID_PARAMETER != RVCMemoryPoolCreate((void *)MemoryBase1, 128, NULL)){
        WriteString("MemoryPoolCreate doesn't handle NULL memoryid.\n");    
        return 0;
    }
    if(RVCOS_STATUS_SUCCESS != RVCMemoryPoolCreate((void *)MemoryBase1, 128, &MemoryPool1)){
        WriteString("MemoryPoolCreate doesn't return success with valid inputs.\n");    
        return 0;
    }
    WriteString("Main RVCMemoryPoolCreate appears OK.\n");
    if(RVCOS_STATUS_SUCCESS != RVCMemoryPoolAllocate(MemoryPool1, 96, (void **)&LocalAllocation)){
        WriteString("MemoryPoolAllocate doesn't return success with valid inputs.\n");    
        return 0;
    }
    if(LocalAllocation != MemoryBase1){
        WriteString("MemoryPoolAllocate allocated from the wrong pool.\n");    
        return 0;        
    }
    if(RVCOS_STATUS_SUCCESS != RVCMemoryPoolQuery(MemoryPool1, &SystemPoolSize)){
        WriteString("MemoryPoolQuery doesn't return success with valid inputs.\n");    
        return 0;
    }
    if(0 != SystemPoolSize){
        WriteString("MemoryPoolQuery doesn't keep track of actual available memory.\n");    
        return 0;        
    }
    WriteString("Main testing RVCMemoryPoolDeallocate.\n");
    if(RVCOS_STATUS_ERROR_INVALID_PARAMETER != RVCMemoryPoolDeallocate(MemoryPool1, NULL)){
        WriteString("MemoryPoolDeallocate doesn't handle NULL base.\n");    
        return 0;
    }
    if(RVCOS_STATUS_ERROR_INVALID_PARAMETER != RVCMemoryPoolDeallocate(MemoryPool1, LocalAllocation + 1)){
        WriteString("MemoryPoolDeallocate doesn't handle zero size.\n");    
        return 0;
    }
    if(RVCOS_STATUS_ERROR_INVALID_ID != RVCMemoryPoolDeallocate(RVCOS_MEMORY_POOL_ID_INVALID, LocalAllocation)){
        WriteString("MemoryPoolDeallocate doesn't handle NULL memoryid.\n");    
        return 0;
    }
    WriteString("Main RVCMemoryPoolDeallocate appears OK.\n");
    WriteString("Main testing RVCMemoryPoolDelete.\n");
    if(RVCOS_STATUS_ERROR_INVALID_ID != RVCMemoryPoolDelete(RVCOS_MEMORY_POOL_ID_INVALID)){
        WriteString("MemoryPoolDelete doesn't handle bad memoryid.\n");    
        return 0;
    }
    if(RVCOS_STATUS_ERROR_INVALID_STATE != RVCMemoryPoolDelete(MemoryPool1)){
        WriteString("MemoryPoolDelete doesn't handle allocated memory pools.\n");    
        return 0;
    }
    if(RVCOS_STATUS_SUCCESS != RVCMemoryPoolDeallocate(MemoryPool1, LocalAllocation)){
        WriteString("MemoryPoolDeallocate doesn't handle valid inputs.\n");    
        return 0;
    }
    if(RVCOS_STATUS_SUCCESS != RVCMemoryPoolDelete(MemoryPool1)){
        WriteString("MemoryPoolDelete doesn't handle valid inputs.\n");    
        return 0;
    }
    WriteString("Main RVCMemoryPoolDelete appears OK.\n");
    if(RVCOS_STATUS_ERROR_INVALID_ID != RVCMemoryPoolAllocate(MemoryPool1, 32, (void **)&LocalAllocation)){
        WriteString("MemoryPoolAllocate doesn't handle bad memoryid.\n");    
        return 0;
    }
    if(RVCOS_STATUS_ERROR_INVALID_ID != RVCMemoryPoolQuery(MemoryPool1, &SystemPoolSize)){
        WriteString("MemoryPoolQuery doesn't handle bad memoryid.\n");    
        return 0;
    }
    WriteString("Main testing RVCThreadID.\n");
    if(RVCOS_STATUS_ERROR_INVALID_PARAMETER != RVCThreadID(NULL)){
        WriteString("ThreadID doesn't handle NULL threadref.\n");    
        return 0;
    }
    if(RVCOS_STATUS_SUCCESS != RVCThreadID(&ThisThreadID)){
        WriteString("ThreadID doesn't handle valid threadref.\n");    
        return 0;
    }
    BadThreadID = (OtherThreadID > ThisThreadID ? OtherThreadID : ThisThreadID) + 16;
    WriteString("Main RVCThreadID appears OK.\n");
    WriteString("Main testing RVCThreadState.\n");
    if(RVCOS_STATUS_ERROR_INVALID_PARAMETER != RVCThreadState(OtherThreadID, NULL)){
        WriteString("ThreadState doesn't handle NULL state.\n");    
        return 0;
    }
    if(RVCOS_STATUS_ERROR_INVALID_ID != RVCThreadState(BadThreadID, &RVCState)){
        WriteString("ThreadState doesn't handle bad thread.\n");    
        return 0;
    }
    if(RVCOS_STATUS_SUCCESS != RVCThreadState(OtherThreadID, &RVCState)){
        WriteString("ThreadState doesn't handle valid inputs.\n");    
        return 0;
    }
    if(RVCOS_THREAD_STATE_CREATED != RVCState){
        WriteString("ThreadState returned the wrong value for state.\n");    
        return 0;        
    }
    WriteString("Main RVCThreadState appears OK.\n");
    WriteString("Main testing RVCTickMS.\n");
    if(RVCOS_STATUS_ERROR_INVALID_PARAMETER != RVCTickMS(NULL)){
        WriteString("TickMS doesn't handle NULL ticksmsref.\n");    
        return 0;
    }
    MSPerTick = 100000;
    if(RVCOS_STATUS_SUCCESS != RVCTickMS(&MSPerTick)){
        WriteString("TickMS doesn't handle valid inputs.\n");    
        return 0;
    }
    if((1 > MSPerTick)||(500 <= MSPerTick)){
        WriteString("TickMS doesn't return proper value for tickmsref.\n");    
        return 0;        
    }
    WriteString("Main RVCTickMS appears OK.\n");
    WriteString("Main testing RVCTickCount.\n");
    if(RVCOS_STATUS_ERROR_INVALID_PARAMETER != RVCTickCount(NULL)){
        WriteString("TickCount doesn't handle NULL tickref.\n");    
        return 0;
    }
    if(RVCOS_STATUS_SUCCESS != RVCTickCount(&CurrentTick)){
        WriteString("TickCount doesn't handle valid inputs.\n");    
        return 0;
    }
    WriteString("Main RVCTickCount appears OK.\n");
    WriteString("Main testing RVCThreadTerminate.\n");
    if(RVCOS_STATUS_ERROR_INVALID_ID != RVCThreadTerminate(BadThreadID,0)){
        WriteString("ThreadTerminate doesn't handle bad thead.\n");    
        return 0;
    }
    if(RVCOS_STATUS_ERROR_INVALID_STATE != RVCThreadTerminate(OtherThreadID,0)){
        WriteString("ThreadTerminate doesn't handle dead thead.\n");    
        return 0;
    }
    WriteString("Main RVCThreadTerminate appears OK.\n");
    WriteString("Main testing RVCMutexCreate.\n");
    if(RVCOS_STATUS_ERROR_INVALID_PARAMETER != RVCMutexCreate(NULL)){
        WriteString("MutexCreate doesn't handle NULL mutexref.\n");    
        return 0;
    }
    if(RVCOS_STATUS_SUCCESS != RVCMutexCreate(&TheMutex)){
        WriteString("MutexCreate doesn't handle valid inputs.\n");    
        return 0;
    }
    BadMutexID = TheMutex + 16;
    WriteString("Main RVCMutexCreate appears OK.\n");
    WriteString("Main testing RVCMutexQuery.\n");
    if(RVCOS_STATUS_ERROR_INVALID_PARAMETER != RVCMutexQuery(TheMutex, NULL)){
        WriteString("MutexQuery doesn't handle NULL ownerref.\n");    
        return 0;
    }
    if(RVCOS_STATUS_ERROR_INVALID_ID != RVCMutexQuery(BadMutexID, &MutexOwner)){
        WriteString("MutexQuery doesn't handle bad mutex.\n");    
        return 0;
    }
    if(RVCOS_STATUS_SUCCESS != RVCMutexQuery(TheMutex, &MutexOwner)){
        WriteString("MutexQuery doesn't handle valid inputs.\n");    
        return 0;
    }
    if(RVCOS_THREAD_ID_INVALID != MutexOwner){
        WriteString("MutexQuery doesn't correct value for owner.\n");    
        return 0;        
    }
    WriteString("Main RVCMutexQuery appears OK.\n");
    WriteString("Main testing RVCThreadSleep.\n");
    if(RVCOS_STATUS_ERROR_INVALID_PARAMETER != RVCThreadSleep(RVCOS_TIMEOUT_INFINITE)){
        WriteString("ThreadSleep doesn't handle RVCOS_TIMEOUT_INFINITE.\n");    
        return 0;
    }
    if(RVCOS_STATUS_SUCCESS != RVCThreadSleep(RVCOS_TIMEOUT_IMMEDIATE)){
        WriteString("ThreadSleep doesn't handle RVCOS_TIMEOUT_IMMEDIATE.\n");    
        return 0;
    }
    RVCTickCount(&CurrentTick);
    do{
        LastTick = CurrentTick;
        RVCTickCount(&CurrentTick);
    }while(LastTick == CurrentTick);
    if(RVCOS_STATUS_SUCCESS != RVCThreadSleep(10)){
        WriteString("ThreadSleep doesn't handle valid input.\n");    
        return 0;
    }
    RVCTickCount(&CurrentTick);
    if(CurrentTick < LastTick + 10){
        WriteString("ThreadSleep doesn't appear to sleep long enough.\n");    
        return 0;
    }
    WriteString("Main RVCThreadSleep appears OK.\n");
    WriteString("Main testing RVCThreadActivate.\n");
    if(RVCOS_STATUS_ERROR_INVALID_ID != RVCThreadActivate(BadThreadID)){
        WriteString("ThreadActivate doesn't handle bad thead.\n");    
        return 0;
    }
    RVCTickCount(&CurrentTick);
    do{
        LastTick = CurrentTick;
        RVCTickCount(&CurrentTick);
    }while(LastTick == CurrentTick);
    LastTick = CurrentTick;
    if(RVCOS_STATUS_SUCCESS != RVCThreadActivate(OtherThreadID)){
        WriteString("ThreadActivate doesn't handle valid inputs.\n");    
        return 0;
    }
    RVCTickCount(&CurrentTick);
    if(CurrentTick != LastTick){
        WriteString("ThreadActivate doesn't schedule properly.\n");    
        return 0;
    }
    WriteString("Main RVCThreadActivate appears OK.\n");
    while(-1 == MemoryAllocationSuccess);
    if(0 == MemoryAllocationSuccess){
        WriteString("Main stacks not allocated from system memory pool\n");  
        return 0;
    }
    while(!TheMutexAcquired);
    WriteString("Main RVCThread appears to have Acquired Mutex.\n");
    
    RVCMutexQuery(TheMutex, &MutexOwner);
    if(MutexOwner != OtherThreadID){
        WriteString("MutexQuery returned wrong owner of mutex.\n");    
        return 0;
    }
    
    WriteString("Main testing RVCMutexAcquire.\n");
    if(RVCOS_STATUS_ERROR_INVALID_ID != RVCMutexAcquire(BadMutexID, RVCOS_TIMEOUT_INFINITE)){
        WriteString("MutexAcquire doesn't handle bad mutex.\n");    
        return 0;
    }
    if(RVCOS_STATUS_FAILURE != RVCMutexAcquire(TheMutex, RVCOS_TIMEOUT_IMMEDIATE)){
        WriteString("MutexAcquire doesn't handle RVCOS_TIMEOUT_IMMEDIATE.\n");    
        return 0;
    }
    RVCTickCount(&CurrentTick);
    do{
        LastTick = CurrentTick;
        RVCTickCount(&CurrentTick);
    }while(LastTick == CurrentTick);
    if(CurrentTick == LastTick + 1){
        WriteString("Scheduler preemption not handled properly.\n");    
        return 0;
    }
    LastTick = CurrentTick;
    if(RVCOS_STATUS_FAILURE != RVCMutexAcquire(TheMutex, 10)){
        WriteString("MutexAcquire doesn't handle timeout properly.\n");    
        return 0;
    }
    RVCTickCount(&CurrentTick);
    if(CurrentTick < LastTick + 10){
        WriteString("MutexAcquire doesn't handle timeout properly, woke too soon.\n");    
        return 0;
    }
    WriteString("Main RVCMutexAcquire appears OK.\n");
    WriteString("Main testing RVCThreadDelete.\n");
    if(RVCOS_STATUS_ERROR_INVALID_ID != RVCThreadDelete(BadThreadID)){
        WriteString("ThreadDelete doesn't handle bad thead.\n");    
        return 0;
    }
    if(RVCOS_STATUS_ERROR_INVALID_STATE != RVCThreadDelete(OtherThreadID)){
        WriteString("ThreadDelete doesn't handle non-dead thead.\n");    
        return 0;
    }
    WriteString("Main RVCThreadDelete appears OK.\n");
    WriteString("Main testing RVCMutexDelete.\n");
    if(RVCOS_STATUS_ERROR_INVALID_ID != RVCMutexDelete(BadMutexID)){
        WriteString("MutexDelete doesn't handle bad thead.\n");    
        return 0;
    }
    if(RVCOS_STATUS_ERROR_INVALID_STATE != RVCMutexDelete(TheMutex)){
        WriteString("MutexDelete doesn't handle held mutexes.\n");    
        return 0;
    }
    WriteString("Main RVCMutexDelete appears OK.\n");
    WriteString("Main testing RVCMutexRelease.\n");
    if(RVCOS_STATUS_ERROR_INVALID_ID != RVCMutexRelease(BadMutexID)){
        WriteString("MutexRelease doesn't handle bad thead.\n");    
        return 0;
    }
    if(RVCOS_STATUS_ERROR_INVALID_STATE != RVCMutexRelease(TheMutex)){
        WriteString("MutexRelease doesn't handle held mutexes.\n");    
        return 0;
    }
    WriteString("Main RVCMutexRelease appears OK.\n");
    WriteString("Main terminating RVCThread.\n");
    if(RVCOS_STATUS_SUCCESS != RVCThreadTerminate(OtherThreadID,0)){
        WriteString("ThreadTerminate doesn't handle valid termination.\n");    
        return 0;
    }
    if(RVCOS_STATUS_SUCCESS != RVCThreadState(OtherThreadID, &RVCState)){
        WriteString("ThreadState doesn't handle valid inputs.\n");    
        return 0;
    }
    if(RVCOS_THREAD_STATE_DEAD != RVCState){
        WriteString("ThreadState returned the wrong value for state.\n");    
        return 0;        
    }
    WriteString("Main RVCThreadTerminate appears OK.\n");
    WriteString("Main testing RVCMutexAcquire\n");
    if(RVCOS_STATUS_SUCCESS != RVCMutexAcquire(TheMutex, RVCOS_TIMEOUT_IMMEDIATE)){
        WriteString("MutexAcquire doesn't handle terminated owners.\n");    
        return 0;
    }
    if(RVCOS_STATUS_SUCCESS != RVCMutexQuery(TheMutex, &MutexOwner)){
        WriteString("MutexQuery doesn't handle valid inputs.\n");    
        return 0;
    }
    if(MutexOwner != ThisThreadID){
        WriteString("MutexQuery returned wrong owner of mutex.\n");    
        return 0;
    }
    WriteString("Main RVCMutexAcquire appears OK.\n");
    WriteString("Main testing RVCThreadDelete.\n");
    if(RVCOS_STATUS_SUCCESS != RVCThreadDelete(OtherThreadID)){
        WriteString("ThreadDelete doesn't handle dead thead.\n");    
        return 0;
    }
    WriteString("Main RVCThreadDelete appears OK.\n");
    
    WriteString("Main testing RVCMutexRelease.\n");
    if(RVCOS_STATUS_SUCCESS != RVCMutexRelease(TheMutex)){
        WriteString("MutexRelease doesn't handle held mutexes.\n");    
        return 0;
    }
    if(RVCOS_STATUS_SUCCESS != RVCMutexQuery(TheMutex, &MutexOwner)){
        WriteString("MutexQuery doesn't handle valid inputs.\n");    
        return 0;
    }
    if(RVCOS_THREAD_ID_INVALID != MutexOwner){
        WriteString("MutexQuery doesn't correct value for owner.\n");    
        return 0;        
    }
    WriteString("Main RVCMutexRelease appears OK.\n");
    WriteString("Main testing RVCMutexDelete.\n");
    if(RVCOS_STATUS_SUCCESS != RVCMutexDelete(TheMutex)){
        WriteString("MutexDelete doesn't handle held mutexes.\n");    
        return 0;
    }
    WriteString("Main RVCMutexDelete appears OK.\n");
    
    WriteString("Main everything appears fine.\nGoodbye\n");
    return 0;
}
