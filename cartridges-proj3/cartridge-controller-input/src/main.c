#include <stdint.h>
#include "RVCOS.h"

int main() {
    SControllerStatus ControllerStatus;
    char OutChar = 'X';
    RVCWriteText("X",1);
    while(1){
        uint32_t Delay = 100000;
        while(Delay){
            Delay--;
        }
        RVCReadController(&ControllerStatus);
        if(ControllerStatus.DRight){
            RVCWriteText("\b ",2);
            RVCWriteText(&OutChar,1);
        }
        else if(ControllerStatus.DLeft){
            RVCWriteText("\b \b\b",4);
            RVCWriteText(&OutChar,1);
        }
        else if(ControllerStatus.DDown){
            RVCWriteText("\b \n",3);
            RVCWriteText(&OutChar,1);
        }
        else if(ControllerStatus.DButton1){
            if('0' < OutChar){
                OutChar--;
                RVCWriteText("\b",1);
                RVCWriteText(&OutChar,1);
            }
        }
        else if(ControllerStatus.DButton2){
            if('~' > OutChar){
                OutChar++;
                RVCWriteText("\b",1);
                RVCWriteText(&OutChar,1);
            }
        }
    }
    
    return 0;
}
