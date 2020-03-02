#ifndef COLOR_H
#define	COLOR_H

#include <xc.h> // include processor files - each processor file is guarded.  

const uint8_t LED_PRESS_COLOR_R = 0;
const uint8_t LED_PRESS_COLOR_G = 1;
const uint8_t LED_PRESS_COLOR_B = 2;
const uint8_t LED_DARK_COLOR_R  = 3;
const uint8_t LED_DARK_COLOR_G  = 4;
const uint8_t LED_DARK_COLOR_B  = 5;

extern uint8_t getColor( uint8_t id, uint8_t kind );


#endif	/* COLOR_H */

