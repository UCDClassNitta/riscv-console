#ifndef ESC_CODES_H
#define ESC_CODES_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "RVCOS.h"

uint32_t esc_codes(const TTextCharacter *buffer, TMemorySize writesize);

#endif