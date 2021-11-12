#include "esc_codes.h"

uint32_t esc_codes(const TTextCharacter *buffer, TMemorySize writesize) {
    switch ((uint32_t)buffer[2]) {
        case 65: { //A
            return (uint32_t)1;
            break;
        }
        case 66: { //B
            return (uint32_t)2;
            break;
        }
        case 67: { //C
            return (uint32_t)3;
            break;
        }
        case 68: { //D
            return (uint32_t)4;
            break;
        }
        case 72: { //H
            return (uint32_t)5;
            break;
        }
        case 50: { //2
            if ((uint32_t)buffer[3] == 'J') { //Ensure it's valid esc sequence
                return (uint32_t)6;
            }
            break;                    
        }
        default: break;
    }
}