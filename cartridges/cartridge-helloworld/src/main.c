#include <stdint.h>
#include "RVCOS.h"

int main() {
  RVCWriteText("Hello World!Hello World!\nHello World!Hello World!Hello World!Hello World!", 72);
  RVCWriteText("Hello World!Hello World!Hallo World!Hello World!Hello World!Hello World!", 72);
  RVCWriteText("Hello World!Hello World!Hello World!Hello World!Hello World!Hello World!", 72);
  RVCWriteText("Hello World!Hello World!Hello World!Hello World!Hello World!Hello World!", 72);
  RVCWriteText("Hello World!Hello World!Hello World!Hello World!Hello World!Hello World!", 72);

  return 0;
}
