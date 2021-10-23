#include <RVCOS.h>

volatile char *VIDEO_MEMORY = (volatile char *)(0x50000000 + 0xFE800);

TStatus RVCWriteText(const TTextCharacter *buffer, TMemorySize writesize){
  const uint32_t stat = 0;
  for (int i = 0; i < writesize; i++)
  {
    VIDEO_MEMORY[i] = buffer[i];
  }

  return stat;
}