#ifndef DEFINE_H
#define	DEFINE_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define PORT_BUTTON RA3 // ボタン入力
#define PORT_LED_R  RA2 // LED 赤出力
#define PORT_LED_G  RA4 // LED 緑出力
#define PORT_LED_B  RA5 // LED 青出力

#define _XTAL_FREQ 8000000

const uint8_t LED_MAX         = 8;


#endif	/* DEFINE_H */

