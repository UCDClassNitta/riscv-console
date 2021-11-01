#include <stdint.h>
#include "RVCOS.h"

int main() {
  RVCWriteText("Hello World!Hello World!\nHello World!Hello World!Hello World!Hello World!", 72);

  SControllerStatus ControllerStatus;
  while (1){
    RVCReadController(&ControllerStatus);
    if (ControllerStatus.DButton1){
      RVCWriteText("dbutton1", 8);
    }
    if (ControllerStatus.DLeft){
      RVCWriteText("dleft", 5);
    }
  }

  return 0;
}
