#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "api.h"
#include <stdlib.h>

//
// Timer constants
#define TIMER_INTERVAL 3  

// Memory-mapped timer registers
#define MTIME_LOW (*((volatile uint32_t *)0x40000008))
#define MTIME_HIGH (*((volatile uint32_t *)0x4000000C))
#define MTIMECMP_LOW (*((volatile uint32_t *)0x40000010))
#define MTIMECMP_HIGH (*((volatile uint32_t *)0x40000014))

//

volatile int global = 42;
volatile uint32_t controller_status = 0;
volatile uint32_t videoToggle = 0;
volatile uint32_t vidIntCtr = 0;
volatile int sprite_color = 1;  // 1 for green, 2 for red

volatile char *VIDEO_MEMORY = (volatile char *)(0x50000000 + 0xF4800);
volatile uint32_t *MODE_REGISTER = (volatile uint32_t *)(0x500F6780);
volatile uint32_t *MEDIUM_PALETTE = (volatile uint32_t *)(0x500F2000);
volatile uint32_t *MEDIUM_CONTROL = (volatile uint32_t *)(0x500F5F00);
volatile uint8_t *MEDIUM_DATA = (volatile uint8_t *)(0x500D0000);
volatile uint32_t *VIDEO_MODE = (volatile uint32_t *)(0x500FF414);

void toggleSpriteColor(void);
void printError(char* errorMessage);

void setUpMediumSprites() {
    // Set up sprite data for two sprites (this assumes sprite 0 and sprite 1 are the ones we're moving)
    for (int i = 0; i < 1024; i++) {
        MEDIUM_DATA[i] = 1; // Set up the first sprite (index 0)
        MEDIUM_DATA[1024 + i] = 1; // Set up the second sprite (index 1)
    }

    // Set the color green for the sprites
    MEDIUM_PALETTE[1] = setSpriteColor(255, 0, 255, 0);

    // Initialize control registers for two sprites
    MEDIUM_CONTROL[0] = mediumControlSetter(0, 0, 0, 0, 0); // First sprite
    *MODE_REGISTER = 1; // Set graphics mode
}

void beginTheGUI();

int moveBox() {
    int a = 4;
    int b = 12;
    int last_global = 42;
    int x_pos = 0;
    int y_pos = 0; // Y position of the sprite, now declared
    int countdown = 1;

    *VIDEO_MODE = 1; // VIDEO MODE ON
    beginTheGUI();

    while (1) {
        global = getTicks();
        if(global != last_global){
            controller_status = getControllerStatus();
            if(controller_status){
                // Update sprite position based on controller_status
                if(controller_status & 0x1 && x_pos - SPEED_INCREASE >= 0){
                    x_pos -= SPEED_INCREASE;
                }
                if(controller_status & 0x2 && y_pos - SPEED_INCREASE >= 0){
                    y_pos -= SPEED_INCREASE; // Updated for Y position
                }
                if(controller_status & 0x4 && y_pos + SPEED_INCREASE <= MAX_Y){
                    y_pos += SPEED_INCREASE; // Updated for Y position
                }
                if(controller_status & 0x8 && x_pos + SPEED_INCREASE <= MAX_X){
                    x_pos += SPEED_INCREASE;
                }
                // Update the sprite control registers with the new position
                MEDIUM_CONTROL[0] = mediumControlSetter(0, x_pos, y_pos, 0, 0);
            }
            last_global = global;
        }
        countdown--;
        if(!countdown){
            global++;
            controller_status = (*((volatile uint32_t *)0x40000018));
            countdown = 100000;
        }
        // Check for timer interrupt and toggle sprite color
        if (MTIME_LOW >= MTIMECMP_LOW && MTIME_HIGH >= MTIMECMP_HIGH) {
            toggleSpriteColor();
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

void toggleSpriteColor(void) {
    if (sprite_color == 1) { //if green
        MEDIUM_PALETTE[1] = setSpriteColor(255, 255, 0, 0);  // Red color
        sprite_color = 2; // red
    } else { //if red
        MEDIUM_PALETTE[1] = setSpriteColor(255, 0, 255, 0);  // Green color
        sprite_color = 1; // green
    }

    // Set the next timer interrupt
    MTIMECMP_LOW = MTIME_LOW + TIMER_INTERVAL;
    if (MTIMECMP_LOW < TIMER_INTERVAL) {
        // Handle carry
        MTIMECMP_HIGH += 1;
    }
}

int main() {
    moveBox();
    return 0;
}
