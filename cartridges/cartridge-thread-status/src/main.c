#include <stdint.h>
#include <stdlib.h>
#include "RVCOS.h"

TThreadID MainThreadID, HighThreadID;

void WriteString(const char *str){
    const char *Ptr = str;
    while(*Ptr){
        Ptr++;
    }
    RVCWriteText(str,Ptr-str);
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
    if(RVCOS_THREAD_STATE_READY != ThreadState){
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
    return 0;
}

int main(){
    TThreadID CurrentThreadID;
    TThreadState ThreadState;

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
    RVCThreadActivate(HighThreadID);
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
