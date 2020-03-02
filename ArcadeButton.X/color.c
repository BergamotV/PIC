#include <stdint.h>
#include "define.h"
#include "distance.h"


const uint8_t color[] = {
 255,   0,   0,  32,   0,   0,
   0, 255,   0,   0,  32,   0,
   0,   0, 255,   0,   0,  32,
 255, 255,   0,  32,  32,   0,
   0, 255, 255,   0,  32,  32,
 255,   0, 255,  32,   0,  32,
  92, 255,   0,  10,  32,   0,
 192, 192, 192,  20,  20,  20,
};

// lightId, LED_PRESS_COLOR_R
uint8_t getColor( uint8_t colIdx, uint8_t kind )
{
    uint8_t index = colIdx *  6 + kind;
    return color[ index ];
}
