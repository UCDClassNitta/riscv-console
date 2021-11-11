#include <stdint.h>
#include "RVCOS.h"

int main() {
    SControllerStatus ControllerStatus;
    char ArrayMove[7] = {' ','\b','\x1B','[','A','X','\b'};
    RVCWriteText("X\b",2);
    while(1){
        uint32_t Delay = 100000;
        while(Delay){
            Delay--;
        }
        RVCReadController(&ControllerStatus);
        if(ControllerStatus.DUp){
            ArrayMove[4] = 'A';
            RVCWriteText(ArrayMove,7);
        }
        else if(ControllerStatus.DRight){
            ArrayMove[4] = 'C';
            RVCWriteText(ArrayMove,7);
        }
        else if(ControllerStatus.DLeft){
            ArrayMove[4] = 'D';
            RVCWriteText(ArrayMove,7);
        }
        else if(ControllerStatus.DDown){
            ArrayMove[4] = 'B';
            RVCWriteText(ArrayMove,7);
        }
        else if(ControllerStatus.DButton1){
            if('0' < ArrayMove[5]){
                ArrayMove[5]--;
                RVCWriteText(ArrayMove+5,2);
            }
        }
        else if(ControllerStatus.DButton2){
            if('~' > ArrayMove[5]){
                ArrayMove[5]++;
                RVCWriteText(ArrayMove+5,2);
            }
        }
        else if(ControllerStatus.DButton3){
            RVCWriteText("\x1B[2J",4);
        }
        else if(ControllerStatus.DButton4){
            RVCWriteText("\x1B[H",3);
            RVCWriteText(ArrayMove+5,2);
        }
    }
    
    return 0;
}
