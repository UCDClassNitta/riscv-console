#include <stdint.h>
#include "RVCOS.h"

void WriteString(const char *str){
    const char *Ptr = str;
    while(*Ptr){
        Ptr++;
    }
    RVCWriteText(str,Ptr-str);
}

char *IntToString(char *buffer, int val){
    if((-10 < val)&&(10 > val)){
        if(0 > val){
            *buffer++ = '-';
            val = -val;
        }
        *buffer++ = '0' + val;
        return buffer;
    }
    buffer = IntToString(buffer,val / 10);
    val %= 10;
    if(0 > val){
        val = -val;
    }
    *buffer++ = '0' + val;
    return buffer;
}

void WriteInts(int val1, int val2){
    char Buffer[32];
    char *Ptr = IntToString(Buffer,val1);
    *Ptr++ = ' ';
    Ptr = IntToString(Ptr,val2);
    *Ptr++ = '\n';
    *Ptr = '\0';
    WriteString(Buffer);
}

TThreadReturn Thread(void *param){
    volatile int *Val = (int *)param;
    uint32_t RunTimeTicks;
    TTick CurrentTick, EndTick;
    
    RVCTickMS(&RunTimeTicks);
    RunTimeTicks = (5000 + RunTimeTicks - 1)/RunTimeTicks;
    RVCTickCount(&CurrentTick);
    EndTick = CurrentTick + RunTimeTicks;
    WriteString("RVCThread Alive\nRVCThread Starting\n");
    while(EndTick > CurrentTick){
        (*Val)++;
        RVCTickCount(&CurrentTick);
    }
    WriteString("RVCThread Done\n");
    return 0;
}

int main() {
    TThreadID RVCThreadID1, RVCThreadID2;
    TThreadState RVCState1, RVCState2;
    volatile int Val1 = 0, Val2 = 0;
    volatile int LocalVal1, LocalVal2;
    WriteString("RVCMain creating threads.\n");
    RVCThreadCreate(Thread, (void *)&Val1, 0x1000, RVCOS_THREAD_PRIORITY_LOW, &RVCThreadID1);
    RVCThreadCreate(Thread, (void *)&Val2, 0x1000, RVCOS_THREAD_PRIORITY_LOW, &RVCThreadID2);
    WriteString("RVCMain activating threads.\n");
    RVCThreadActivate(RVCThreadID1);
    RVCThreadActivate(RVCThreadID2);
    WriteString("RVCMain Waiting\n");
    do{
        LocalVal1 = Val1; 
        LocalVal2 = Val2;
        RVCThreadState(RVCThreadID1, &RVCState1);
        RVCThreadState(RVCThreadID2, &RVCState2);
        WriteInts(LocalVal1, LocalVal2);
        RVCThreadSleep(2);
    }while((RVCOS_THREAD_STATE_DEAD != RVCState1)||(RVCOS_THREAD_STATE_DEAD != RVCState2));
    WriteString("RVCMain Done\n");
    WriteString("Goodbye\n");
    return 0;
}
