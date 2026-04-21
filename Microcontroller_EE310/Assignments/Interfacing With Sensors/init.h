#ifndef INIT_H
#define INIT_H

#include <xc.h>
#include <stdint.h>

#define _XTAL_FREQ 1000000UL

void OSCILLATOR_Init(void)
{
    OSCFRQ = 0x00;
}

void PORT_Init(void)
{
    ANSELA = 0x00;
    ANSELB = 0x00;
    ANSELD = 0x00;

    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0;
    TRISAbits.TRISA2 = 0;

    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB2 = 1;

    TRISD = 0x00;

    LATAbits.LATA0 = 0;
    LATAbits.LATA1 = 0;
    LATAbits.LATA2 = 0;
    LATD = 0x00;

    WPUBbits.WPUB2 = 1;
}

void INTERRUPT_Init(void)
{
    IOCBNbits.IOCBN2 = 1;
    IOCBFbits.IOCBF2 = 0;

    PIR0bits.IOCIF = 0;
    PIE0bits.IOCIE = 1;

    INTCON0bits.GIE = 1;
}

void SYSTEM_Init(void)
{
    OSCILLATOR_Init();
    PORT_Init();
    INTERRUPT_Init();
}

#endif
