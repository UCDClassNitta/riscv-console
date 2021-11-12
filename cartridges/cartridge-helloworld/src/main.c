#include <stdint.h>
#include "RVCOS.h"

/**
 * VT-100 Escape Codes
 * RVCWriteText("\x1B[A") - Cursor Up
 * RVCWriteText("\x1B[B") - Cursor Down
 * RVCWriteText("\x1B[C") - Cursor Right
 * RVCWriteText("\x1B[D") - Cursor Left
 * RVCWriteText("\x1B[1;1H") - Move cursor to ln:1 col:1
 * RVCWriteText("\x1B[H") - Cursor to Upper Left
 * RVCWriteText("\x1B[2J") - Erase Screen, leave Cursor  
 * */

int main() {
  RVCWriteText("\x1B[65;37H", 9);
  return 0;
}
