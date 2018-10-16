#include <pic12f1572.h>
#include <stdio.h>


// CONFIG1
#pragma config FOSC = INTOSC    //  (INTOSC oscillator; I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = OFF      // PLL Enable (4x PLL disabled)
#pragma config STVREN = ON     // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will not cause a Reset)
#pragma config BORV = HI        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), high trip point selected.)
#pragma config LPBOREN = OFF    // Low Power Brown-out Reset enable bit (LPBOR is disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

void ADC_result()
{
    ADCON0bits.GO=1;                // ADC start conversion
    while(ADCON0bits.GO);           // wait for conversion done
}

void PWM()
{
    PWM1CLKCON = 0b00000001;          // Set HFINTOSC clock, no prescaler.
    PWM1CON = 0b01100000;             // PWM control (standard mode).
    PWM1PH = 0;                       // Phase.
    PWM1PR = 1024;                    // Period.
    APFCONbits.P1SEL = 1;             // PWM1 on RA5.
    PWMENbits.PWM1EN_A=1;
}

void ADC_set()
{
    ANSELAbits.ANSA2=1;             // Analog input on PIN RA2
    ADCON0bits.CHS=0b10;            // Analog channel RA2
    ADCON1bits.ADFM=1;              // ADC set to ADRESL
    ADCON0bits.ADON=1;              // ADC enabled
    ADCON1bits.ADCS=0b11;           // ADC conversion clock select as FRC
}

void main(void) 

{
    TRISAbits.TRISA=0b100;          // I/O RA2 input
    CM1CON0bits.C1ON=0;             // Disable Comaprator
    OSCCON = 0b01111010;            // 16 Mhz oscillator.
    PWM();
    ADC_set();
    
while(1)
    {
        ADC_result();
        PWM1DC=ADRES;
        PWMLDbits.PWM1LDA_A=1;
    }
}


