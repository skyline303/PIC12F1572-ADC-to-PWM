#include <pic12f1572.h>
#include <stdio.h>
#include <xc.h>

#define _XTAL_FREQ 16000000


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

void delay()
{
    for(unsigned int a=0; a==65535; a++)
                for(unsigned int b=0; b==65535; b++)
                NOP();
}
void ADC_result()
{
    ADCON0bits.GO=1;                // ADC start conversion
    while(ADCON0bits.GO);           // wait for conversion done
}

void PWM()
{
    PWM1CLKCON = 0b00000000;          // Set HFINTOSC clock, no prescaler.
    PWM1CON = 0b01100000;             // PWM control (standard mode).
    PWM1PH = 0;                       // Phase.
    PWM1PR = 1737;                    // Period.
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
    TRISAbits.TRISA4=0;             // I/O RA4 LED
    CM1CON0bits.C1ON=0;             // Disable Comaprator
    OSCCON = 0b01111010;            // 16 Mhz oscillator.
    PWM();
    ADC_set();
    
while(1)
    {
        ADC_result();
        if(ADRES>=441 && ADRES<=750)
        {
            PWM1DC=ADRES+1000;
            LATAbits.LATA4=0;
        }
        else if(ADRES>750)
            {
                LATAbits.LATA4=1;
                PWM1DC=1737;
            }
        else
        {
            LATAbits.LATA4=!LATAbits.LATA4;
            //delay();
            __delay_ms(500);
            PWM1DC=0;
        }
        PWMLDbits.PWM1LDA_A=1;
    }
    
}