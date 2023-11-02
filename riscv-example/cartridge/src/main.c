#include <stdint.h>
#include <stdio.h>
#include <string.h>

volatile int global = 42;
volatile uint32_t controller_status = 0;

volatile char *VIDEO_MEMORY = (volatile char *)(0x50000000 + 0xF4800);
volatile uint32_t *MODE_REGISTER = (volatile uint32_t *)(0x500F6780);
volatile uint32_t *MEDIUM_PALETTE = (volatile uint32_t *)(0x500F2000);
volatile uint32_t *MEDIUM_CONTROL = (volatile uint32_t *)(0x500F5F00);
volatile uint8_t *MEDIUM_DATA = (volatile uint8_t *)(0x500D0000);

uint32_t mediumControlSetter(uint8_t palette, int16_t x, int16_t y, int8_t z, uint8_t index);

uint32_t setSpriteColor(uint16_t alpha, uint16_t red, uint16_t green, uint16_t blue);

void printError(char* errorMessage);

void setUpMediumSprites();

void beginTheGUI();

int main() {
    int a = 4;
    int b = 12;
    int last_global = 42;
    int x_pos = 0;
    int countdown = 1;

    beginTheGUI();
    
    while (1) {
        int c = a + b + global;
        if(global != last_global){
            if(controller_status){
                if(controller_status & 0x1){
                    if(x_pos & 0x3F){
                        x_pos--;
                    }
                }
                if(controller_status & 0x2){
                    if(x_pos >= 0x40){
                        x_pos -= 0x40;
                    }
                }
                if(controller_status & 0x4){
                    if(x_pos < 0x8C0){
                        x_pos += 0x40;
                    }
                }
                if(controller_status & 0x8){
                    if((x_pos & 0x3F) != 0x3F){
                        x_pos++;
                    }
                }
                MEDIUM_CONTROL[0] = mediumControlSetter(0, (x_pos & 0x3F)<<3, (x_pos>>6)<<3, 0, 0);
            }
            last_global = global;
        }
        countdown--;
        if(!countdown){
            global++;
            controller_status = (*((volatile uint32_t *)0x40000018));
            countdown = 100000;
        }
    }
    return 0;
}

uint32_t mediumControlSetter(uint8_t palette, int16_t x, int16_t y, int8_t z, uint8_t index) {
    return (((uint32_t)index)<<24) | (((uint32_t)z)<<21) | (((uint32_t)y+32)<<12) | (((uint32_t)x+32)<<2) | (palette & 0x3);
}

uint32_t setSpriteColor(uint16_t alpha, uint16_t red, uint16_t green, uint16_t blue) {
    if (alpha > 255 || red > 255 || green > 255 || blue > 255) {
        printError("Color values too high");
        return 0;
    } else if (alpha < 0 || red < 0 || green < 0 || blue < 0) {
        printError("Color values too low");
        return 0;
    }
    return (((uint32_t)alpha)<<24) | (((uint32_t)red)<<16) | (((uint32_t)green)<<8) | ((uint32_t)blue);
}

void printError(char* errorMessage) {
    *MODE_REGISTER = 0;
    for (int i = 0; i < strlen(errorMessage); i++) {
        VIDEO_MEMORY[i] = errorMessage[i];
    }
}

void beginTheGUI() {
    setUpMediumSprites();
}

void setUpMediumSprites() {
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 32; x++) {
            MEDIUM_DATA[1024+y*32+x] = 1;
        }
    }

    // for (int y = 0; y < 32; y++) {
    //     for (int x = 0; x < 32; x++) {
    //         MEDIUM_DATA[y*32+x] = 1; // These are setting the colors
    //     }
    // }

    MEDIUM_PALETTE[1] = setSpriteColor(255, 0, 255, 0);
    MEDIUM_PALETTE[2] = setSpriteColor(255, 0, 255, 255);
    MEDIUM_CONTROL[0] = mediumControlSetter(0,0,0,0,0);
    MEDIUM_CONTROL[1] = mediumControlSetter(0,64,64,0,0);
    *MODE_REGISTER = 1;
}
