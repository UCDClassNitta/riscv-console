#include <stdint.h>
#include <stdlib.h>
#include "RVCOS.h"

#define EXPECTED_RETURN             5

TThreadID MainThreadID, LowThreadID, HighThreadID;

void WriteString(const char *str){
    const char *Ptr = str;
    while(*Ptr){
        Ptr++;
    }
    RVCWriteText(str,Ptr-str);
}

TThreadReturn LowPriorityThread(void *param){
    TThreadID CurrentThreadID;
    TThreadState ThreadState;
    TThreadReturn ReturnValue;

    WriteString("Low Thread:    ");
    RVCThreadID(&CurrentThreadID);
    if(CurrentThreadID != LowThreadID){
        WriteString("Invalid Thread ID\n");
        return 1;
    }
    WriteString("Valid Thread ID\n");
    WriteString("Checking Main: ");
    RVCThreadState(MainThreadID,&ThreadState);
    if(RVCOS_THREAD_STATE_WAITING != ThreadState){
        WriteString("Invalid State\n");
        return 1;
    }
    WriteString("Valid State\n");
    WriteString("Checking This: ");
    RVCThreadState(LowThreadID,&ThreadState);
    if(RVCOS_THREAD_STATE_RUNNING != ThreadState){
        WriteString("Invalid State\n");
        return 1;
    }
    WriteString("Valid State\n");
    RVCThreadActivate(HighThreadID);
    RVCThreadWait(HighThreadID,&ReturnValue);
    WriteString("Low Awake:     ");
    if(EXPECTED_RETURN != ReturnValue){
        WriteString("Invalid Return\n");
        return 0;
    }
    WriteString("Valid Return\n");
    RVCThreadTerminate(LowThreadID,ReturnValue);

    return 0;
}

TThreadReturn HighPriorityThread(void *param){
    TThreadID CurrentThreadID;
    TThreadState ThreadState;

    WriteString("High Thread:   ");
    RVCThreadID(&CurrentThreadID);
    if(CurrentThreadID != HighThreadID){
        WriteString("Invalid Thread ID\n");
        return 1;
    }
    WriteString("Valid Thread ID\n");
    WriteString("Checking Main: ");
    RVCThreadState(MainThreadID,&ThreadState);
    if(RVCOS_THREAD_STATE_WAITING != ThreadState){
        WriteString("Invalid State\n");
        return 1;
    }
    WriteString("Valid State\n");
    WriteString("Checking This: ");
    RVCThreadState(HighThreadID,&ThreadState);
    if(RVCOS_THREAD_STATE_RUNNING != ThreadState){
        WriteString("Invalid State\n");
        return 1;
    }
    WriteString("Valid State\n");
    return EXPECTED_RETURN;
}

int main(){
    TThreadID CurrentThreadID;
    TThreadState ThreadState;
    TThreadReturn ReturnValue;

    WriteString("Main Thread:   ");
    RVCThreadID(&MainThreadID);
    RVCThreadState(MainThreadID,&ThreadState);
    if(RVCOS_THREAD_STATE_RUNNING != ThreadState){
        WriteString("Invalid State\n");
        return 1;
    }
    WriteString("Valid State\n");
    RVCThreadCreate(HighPriorityThread,NULL,2048,RVCOS_THREAD_PRIORITY_HIGH,&HighThreadID);
    WriteString("High Created:  ");
    RVCThreadState(HighThreadID,&ThreadState);
    if(RVCOS_THREAD_STATE_CREATED != ThreadState){
        WriteString("Invalid State\n");
        return 1;
    }
    WriteString("Valid State\n");
    RVCThreadCreate(LowPriorityThread,NULL,2048,RVCOS_THREAD_PRIORITY_LOW,&LowThreadID);
    WriteString("Low Created:   ");
    RVCThreadState(LowThreadID,&ThreadState);
    if(RVCOS_THREAD_STATE_CREATED != ThreadState){
        WriteString("Invalid State\n");
        return 1;
    }
    WriteString("Valid State\n");
    RVCThreadActivate(LowThreadID);
    WriteString("Low Activated: ");
    RVCThreadState(LowThreadID,&ThreadState);
    if(RVCOS_THREAD_STATE_READY != ThreadState){
        WriteString("Invalid State\n");
        return 1;
    }
    WriteString("Valid State\n");
    RVCThreadWait(LowThreadID,&ReturnValue);
    
    WriteString("Checking Low:  ");
    if(EXPECTED_RETURN != ReturnValue){
        WriteString("Invalid Return\n");
        return 1;
    }
    RVCThreadState(LowThreadID,&ThreadState);
    if(RVCOS_THREAD_STATE_DEAD != ThreadState){
        WriteString("Invalid State\n");
        return 1;
    }
    WriteString("Valid Return and State\n");

    WriteString("Checking High: ");
    RVCThreadState(HighThreadID,&ThreadState);
    if(RVCOS_THREAD_STATE_DEAD != ThreadState){
        WriteString("Invalid State\n");
        return 1;
    }
    WriteString("Valid State\n");
    RVCThreadID(&CurrentThreadID);
    if(CurrentThreadID != MainThreadID){
        WriteString("Invalid Main Thread ID\n");
        return 1;
    }
    WriteString("Main Exiting\n");
    
    return 0;
}
