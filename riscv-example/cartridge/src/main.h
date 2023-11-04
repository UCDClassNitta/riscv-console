void setLargeSpriteControl(uint32_t idx, uint32_t h, uint32_t w, uint32_t x, uint16_t y, uint32_t palette);
int setLargeSpriteDataImage(uint32_t idx, uint8_t * addr);
uint32_t setSpriteColor(uint16_t alpha, uint16_t red, uint16_t green, uint16_t blue);
uint32_t* regAddrFromMemHandle(uint32_t mem_handle);

#define SCREEN_WIDTH 512  // Define the screen width (example value)
#define SCREEN_HEIGHT 288 // Define the screen height (example value)
#define SPRITE_WIDTH 32   // Define the sprite width
#define SPRITE_HEIGHT 32  // Define the sprite height
#define MAX_X (SCREEN_WIDTH - SPRITE_WIDTH)
#define MAX_Y (SCREEN_HEIGHT - SPRITE_HEIGHT)
#define SPEED_INCREASE 50  // Change this value to control speed
#define INIT_X 90
#define INIT_Y 30
#define PIXEL_0_SIZE 64 * 64
uint8_t pixel_0[PIXEL_0_SIZE];

uint32_t mediumControlSetter(uint8_t palette, int16_t x, int16_t y, int8_t z, uint8_t index);
uint32_t setSpriteColor(uint16_t alpha, uint16_t red, uint16_t green, uint16_t blue);