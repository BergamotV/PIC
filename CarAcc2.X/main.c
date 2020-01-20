// ==============================================================
// 機能概要：
// 車の Acc が On の時は、Acc2 は常時 On
// 車の Acc が Off になってから、5時間経った後にAcc2 を Off にする。
// 車の Acc が Off の時に、Reset ボタンを押すと、Acc2 を強制的に Off にする 
// ==============================================================

// PIC12F1840 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT enabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = OFF      // PLL Enable (4x PLL disabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (Low-voltage programming enabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdbool.h>

#define _XTAL_FREQ 31000      // 31kHz

#define IN_ACC   RA5  // 車のアクセサリー電源
#define IN_RESET RA4  // リセットボタン
#define OUT_ACC2 RA0  // Acc2の制御出力


const long t1cycle = 65536 - (31000/4); // 内部クロック31k,分周比1/4
const int delaySeconds = 5 * 60 * 60;   // 5時間
//const int delaySeconds = 5;   // テスト用：5秒

int timer;
bool enableAcc2;


// システム全般の初期化
void initSystem()
{
    OSCCONbits.SPLLEN = 0;
    OSCCONbits.IRCF   = 0;          // osc31kHz選択(バッテリー節約のため)
    OSCCONbits.SCS    = 0;
    PORTA  = 0x00;                  // PORTAを初期化
    ANSELA = 0b00000000;            // アナログ入力モードを無効に
    TRISA  = 0b00110000;            // PORTAの入出力設定:RA5, RA4 が入力
    WPUA   = 0b00000000;            // WEAK PULL-UP PORTA REGISTER
    // 1秒に1度割り込みが走るようにする
    T1CONbits.T1CKPS  = 0;          // プリスケーラ
    T1CONbits.T1OSCEN = 0;          // 外部発振回路の作動
    T1CONbits.nT1SYNC = 1;          // 外部入力の内部クロックへの同期
    T1CONbits.TMR1CS  = 0;          // 内部クロックを使用する (intosc/4))
    T1CONbits.TMR1ON  = 1;          // タイマーを動作させる
    TMR1IE = 0;                     // タイマー１割り込みを禁止
    TMR1IF = 0;                     //タイマー１割り込みフラグを０にする
    INTCONbits.GIE = 1;             // グローバル割り込みを許可
    INTCONbits.PEIE = 1;            // 割り込みを許可
}

// Timer1 カウンタの設定
void restartTimer()
{
    timer  = delaySeconds;          //待つ秒数のリセット
    TMR1   = t1cycle;               //タイマーのカウンタリセット
}

// Acc2 を On にする
void OnAccessary2( void )
{
    restartTimer();
    enableAcc2 = true;
    TMR1IE = 1;                     // タイマー１割り込みを許可する
}

// Acc2 を Off にする
void OffAccessary2( void )
{
    enableAcc2 = false;
    TMR1IE = 0;                     // タイマー１割り込みを無効化する
}

// 割り込み処理(1秒ごとに呼ばれる)
void __interrupt() interruptFunc( void )
{
    GIE = 0;
    if(TMR1IF == 1)                 //タイマー１の割り込みか？
    {
        TMR1 = t1cycle;             //タイマー１のカウンタ初期化

        if ( --timer <= 0 )
        {
            // 時間が経過した
            OffAccessary2();
        }
        TMR1IF = 0;                 //タイマー１割り込みフラグを落とす
    }
    GIE = 1;
}
            
// main
void main( void )
{
    initSystem();

    enableAcc2 = false;
 
    while(1)
    {
        if ( IN_ACC == 1 )
        {
            // Acc が On の場合は、常にタイマーをリセット
            OnAccessary2();
        }
        else if ( IN_RESET == 1 )
        {
            // リセットボタンが押された時は、Acc2 を Off に
            OffAccessary2();
        }

        OUT_ACC2 = enableAcc2 ? 1 : 0;

        __delay_ms(50);
    }
}

