#include "RVCOS.h"
#include <string.h>

TMutexID MutexHigh, MutexMedium, MutexLow, MutexMain;

void WriteString(const char *str){
    const char *Ptr = str;
    while(*Ptr){
        Ptr++;
    }
    RVCWriteText(str,Ptr-str);
}

TThreadReturn ThreadHigh(void *param){
    WriteString("ThreadHigh Alive\n");
    RVCMutexAcquire(MutexHigh, RVCOS_TIMEOUT_INFINITE);
    WriteString("ThreadHigh Awake\n");
    return 0;
}

TThreadReturn ThreadMedium(void *param){
    WriteString("ThreadMedium Alive\n");
    RVCMutexAcquire(MutexMedium, RVCOS_TIMEOUT_INFINITE);
    WriteString("ThreadMedium Awake\n");
    return 0;
}

TThreadReturn ThreadLow(void *param){
    RVCMutexAcquire(MutexMain, RVCOS_TIMEOUT_INFINITE);
    WriteString("ThreadLow Alive\n");
    RVCMutexAcquire(MutexLow, RVCOS_TIMEOUT_INFINITE);
    WriteString("ThreadLow Awake\n");
    RVCMutexRelease(MutexMain);
    return 0;
}

int main(){
    TThreadID ThreadIDHigh, ThreadIDMedium, ThreadIDLow;
    
    WriteString("Main creating threads.\n");
    RVCThreadCreate(ThreadLow, NULL, 0x1000, RVCOS_THREAD_PRIORITY_LOW, &ThreadIDLow);
    RVCThreadCreate(ThreadMedium, NULL, 0x1000, RVCOS_THREAD_PRIORITY_NORMAL, &ThreadIDMedium);
    RVCThreadCreate(ThreadHigh, NULL, 0x1000, RVCOS_THREAD_PRIORITY_HIGH, &ThreadIDHigh);
    WriteString("Main creating mutexes.\n");
    RVCMutexCreate(&MutexMain);
    RVCMutexCreate(&MutexLow);
    RVCMutexCreate(&MutexMedium);
    RVCMutexCreate(&MutexHigh);
    WriteString("Main locking mutexes.\n");
    RVCMutexAcquire(MutexLow, RVCOS_TIMEOUT_INFINITE);
    RVCMutexAcquire(MutexMedium, RVCOS_TIMEOUT_INFINITE);
    RVCMutexAcquire(MutexHigh, RVCOS_TIMEOUT_INFINITE);
    WriteString("Main activating processes.\n");
    RVCThreadActivate(ThreadIDLow);
    RVCThreadActivate(ThreadIDMedium);
    RVCThreadActivate(ThreadIDHigh);
    WriteString("Main releasing mutexes.\n");
    RVCMutexRelease(MutexLow);
    RVCMutexRelease(MutexMedium);
    RVCMutexRelease(MutexHigh);
    WriteString("Main acquiring main mutex.\n");
    RVCMutexAcquire(MutexMain, RVCOS_TIMEOUT_INFINITE);
    WriteString("Main acquired main mutex.\nGoodbye\n");
    return 0;
}

