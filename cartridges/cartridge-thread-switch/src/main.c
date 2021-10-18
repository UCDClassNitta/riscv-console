#include <stdint.h>
#include <stdlib.h>
#include "RVCOS.h"

TThreadReturn HighPriorityThread(void *param){
    RVCWriteText("High Thread\n",12);

    return 0;
}

int main(){
    TThreadID HighThreadID;

    RVCWriteText("Main Thread\n",12);
    RVCThreadCreate(HighPriorityThread,NULL,2048,RVCOS_THREAD_PRIORITY_HIGH,&HighThreadID);
    RVCWriteText("High Created\n",13);
    RVCThreadActivate(HighThreadID);
    RVCWriteText("Main Exiting\n",13);
    
    return 0;
}
