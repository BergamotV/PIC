/* 
 * File:   main.c
 * Author: nagasawayoshikazu
 *
 * Created on January 24, 2020, 12:28 AM
 */

#include <stdint.h>
#include <stdbool.h>
#include "define.h"
#include "settings.h"
#include "distance.h"
#include "color.h"


uint8_t min_lumi_r = 8;
uint8_t min_lumi_g = 8;
uint8_t min_lumi_b = 8;

uint8_t lumi_r = 0;
uint8_t lumi_g = 0;
uint8_t lumi_b = 0;
uint8_t button_old = 0;

// 通信用
uint8_t myId = 0;
bool    enableResendData = false;
uint8_t resendData = 0;
bool    enableMineData = false;
uint8_t mineData = 0;

#define LIGHT_BUFFER_MAX 4

uint8_t  _lightDelay [LIGHT_BUFFER_MAX];
uint8_t  _lightId    [LIGHT_BUFFER_MAX];
bool     _lightEnable[LIGHT_BUFFER_MAX];


/*
 * 割り込み処理
 */
void __interrupt() interruptFunc( void )
{
    if ( PIR1bits.RCIF )            // UART受信割り込みであれば
    {
        // 隣からデータが来た
        // バッファが1バイトしかないので、できる限り何もしない
        uint8_t data = RCREG;

        if ( (data & 0x0f) != myId )
        {
            // 隣にそのまま送る
            enableResendData = true;
            resendData = data;
        }

        PIR1bits.RCIF = 0;          // UART受信割り込みフラグクリア
    }
}

/*
 * ライトの登録
 */
void RegistLighting( int lightId )
{
    for ( int i=0 ; i<LIGHT_BUFFER_MAX ; i++ )
    {
        if ( !_lightEnable[i] )
        {
            _lightEnable[i] = true;
            _lightDelay[i]  = getDistance( myId, lightId ) / 2;
            _lightId[i]     = lightId;
            return;
        }
    }
}


void UpdateButton()
{
    uint8_t b = PORT_BUTTON;
    if ( b != button_old )
    {
        button_old = b;
        if ( button_old == 0 )
        {
            // ボタンを押した！
            RegistLighting( myId );
            mineData = myId;
            enableMineData = true;
        }
    }
}


void UpdateLuminusDecay()
{
    if ( min_lumi_r < lumi_r )
        lumi_r -= ((lumi_r>>5)+1);
    else
        lumi_r = min_lumi_r;

    if ( min_lumi_g < lumi_g )
        lumi_g -= ((lumi_g>>5)+1);
    else
        lumi_g = min_lumi_g;

    if ( min_lumi_b < lumi_b )
        lumi_b -= ((lumi_b>>5)+1);
    else
        lumi_b = min_lumi_b;
}



void UpdateReceivedData()
{
    if ( !PIR1bits.TMR1IF )
    {
        if ( enableResendData == true )
        {
            uint8_t data = resendData;
            enableResendData = false;
        
            // ここで、隣に送信
            TXREG = data;

            if ( (data & 0xf0) == 0 )
            {
                // 別の押された処理
                RegistLighting( (data & 0x0f) );
            }
        }
        else if ( enableMineData == true )
        {
            TXREG = mineData;
            enableMineData = false;
        }
    }
}


/*
 * 待機中のライト
 */
void UpdateLightingBuffer()
{
    for ( int i=0 ; i<LIGHT_BUFFER_MAX ; i++ )
    {
        if ( _lightEnable[i] )
        {
            if ( !_lightDelay[i]-- )
            {
                // 発光開始
                _lightEnable[i] = false;
                uint8_t lightId = _lightId[i];
                uint16_t r = lumi_r + getColor( lightId, LED_PRESS_COLOR_R );
                uint16_t g = lumi_g + getColor( lightId, LED_PRESS_COLOR_G );
                uint16_t b = lumi_b + getColor( lightId, LED_PRESS_COLOR_B );
                lumi_r = r > 255 ? 255 : r;
                lumi_g = g > 255 ? 255 : g;
                lumi_b = b > 255 ? 255 : b;
            }
        }
    }
}


/*
 * メイン
 */
int main( void )
{
    myId = eeprom_read( 0 );

    initSystem();
    initSerial();

    min_lumi_r = getColor( myId, LED_DARK_COLOR_R );
    min_lumi_g = getColor( myId, LED_DARK_COLOR_G );
    min_lumi_b = getColor( myId, LED_DARK_COLOR_B );

    lumi_r = min_lumi_r;
    lumi_g = min_lumi_g;
    lumi_b = min_lumi_b;
    
    uint8_t refVal = 0;
    uint8_t mulTimer = 0;
    
    // PWM により、LEDの明るさを調整するループに入る
    while( 1 )
    {
        UpdateReceivedData();

        if ( !(refVal & 0xff) )
        {
            UpdateButton();
            UpdateLightingBuffer();

            mulTimer++;
            if ( !(mulTimer & 0x01) )
            {
                UpdateLuminusDecay();
            }
        }

        // ソフトウェアで PWM 的な動作を行う
        PORT_LED_R = refVal < lumi_r ? 0 : 1;
        PORT_LED_G = refVal < lumi_g ? 0 : 1;
        PORT_LED_B = refVal < lumi_b ? 0 : 1;
        refVal += 1;
    }
}

