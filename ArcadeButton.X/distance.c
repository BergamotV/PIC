
#include <stdint.h>
#include "define.h"
#include "distance.h"


const uint8_t distance[] = {
   0,  40,  80, 120,  40,  56,  89, 126,
  40,   0,  40,  80,  56,  40,  56,  89,
  80,  40,   0,  40,  89,  56,  40,  56,
 120,  80,  40,   0, 126,  89,  56,  40,
  40,  56,  89, 126,   0,  40,  80, 120,
  56,  40,  56,  89,  40,   0,  40,  80,
  89,  56,  40,  56,  80,  40,   0,  40,
 126,  89,  56,  40, 120,  80,  40,   0,
};


uint8_t getDistance( uint8_t myId, uint8_t lightId )
{
    uint8_t index = myId *  LED_MAX + lightId;
    return distance[ index ];
}
