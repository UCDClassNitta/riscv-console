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