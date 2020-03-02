
// PIC12F1840 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = HS        // Oscillator Selection (HS Oscillator, High-speed crystal/resonator connected between OSC1 and OSC2 pins)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = ON       // PLL Enable (4x PLL enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include "settings.h"


void initSystem()
{
    OSCCON = 0b11110000;

    // ポートの I/O初期化
    TRISA  = 0b00110110;
    ANSELA = 0x00; // All pins digital
    ADCON0 = 0x00; // Disable ADC
    CM1CON0bits.C1ON = 0; // Disable Comparator
}

void initSerial()
{
    // UART の初期化
    TRISA0 = 0; // Set RA0 as o/p pin.
    TRISA1 = 1; // Set RA1 as i/p pin.
    PIR1bits.RCIF = 0; // Clear UART receive flag.
    PIR1bits.TXIF = 0; // Clear UART transmit flag.
    // SPBRGH = 17 >> 8; // For 9600 Baud and with 8 Mhz Crystal
    // SPBRGL = 17 & 0xFF;
    SPBRG = 17;
    APFCONbits.RXDTSEL = 0; // RA1 as RX Pin
    APFCONbits.TXCKSEL = 0; // RA0 as TX Pin
    WPUAbits.WPUA0 = 1; // Enable weak pull up on RA0
    WPUAbits.WPUA1 = 1; // Enable weak pull up on RA1
    TXSTAbits.TX9= 0; // 8-BIT DATA MODE
    TXSTAbits.TXEN = 1; // ENABLE TRANSMITTER
    TXSTAbits.SYNC = 0; // ENABLE ASYNCHRONOUS MODE
    TXSTAbits.SENDB = 1; // SYNC BREAK TRANSMISSION COMPLETED
    TXSTAbits.BRGH =0; // SELECT HIGH BAUD RATE
    BAUDCONbits.SCKP = 0; // DON'T INVERT POLARITY
    BAUDCONbits.BRG16 = 0; // 8-BIT BAUDRATE GENERATOR
    BAUDCONbits.ABDEN = 0; // AUTO BAUD RATE DETECT DISABLE
    RCSTAbits.RX9 = 0; // ENABLE 8-BIT RECEPTION
    RCSTAbits.CREN = 1; // ENABLE RECEIVER
    RCSTAbits.FERR = 0;
    RCSTAbits.OERR = 0;
    RCSTAbits.SPEN = 1; // ENABLE SERIAL PORT
    PIE1bits.RCIE = 1; // Enable USART RX Interrupt. 

    INTCON = 0x0; // Initialize interrupt register.
    INTCONbits.PEIE = 1; // Enable Peripheral Interrupt
    INTCONbits.GIE = 1; // Enable Global Interrupt
}
