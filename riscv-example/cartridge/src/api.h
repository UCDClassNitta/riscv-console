/**
 * Returns number of ticks that have elapsed since the system was powered on.
 * @return Ticks
*/
uint32_t getTicks(void);

/**
 * Returns the Multi-Button Controller Status Register.
 * @return controller
*/
uint32_t getControllerStatus(void);

/**
 * @return Video Toggle variable 
*/
uint32_t getVideoToggle(void);

/**
 * Returns the number of video interrupts since the system was powered on.
 * @return Video Interrupt counter
*/
uint32_t getVidIntCtr(void);

/**
 * Calcuates and returns the medium sprite control.
 * 
  */
uint32_t calcMediumSpriteControl(int32_t x, int32_t y, uint32_t w, uint32_t h, uint32_t p);

/**
 * Inserts the parameter "mediumSpriteCtrl" into the Large Sprite Control memory 
 * at the specified large sprite index
 * @param spriteIdx index into the memory map of the large sprite controls
 * @param mediumSpriteCtrl control data of the -CORRECT- LARGE SPRITE CONTROL format 
  */
uint32_t setMediumSpriteControl(uint32_t spriteIdx, uint32_t mediumSpriteCtrl);

/**
 * Sets the medium sprite control data at the spriteIdx to 0x0
 * @return 1 on success
*/
uint32_t deleteMediumSprite(uint32_t spriteIdx);

/**
 * Writes some int num to text position 17 (grows left as it gets bigger)
 * @return 1 on success
*/
uint32_t writeIntToScreen(int num);