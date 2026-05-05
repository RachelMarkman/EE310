
#include <xc.h>
#include <stdio.h>

#define _XTAL_FREQ 4000000UL

#pragma config FEXTOSC = OFF
#pragma config RSTOSC = HFINTOSC_1MHZ
#pragma config CLKOUTEN = OFF
#pragma config WDTE = OFF
#pragma config LVP = ON
#pragma config MCLRE = EXTMCLR
#pragma config MVECEN = OFF

#define RS LATDbits.LATD0
#define EN LATDbits.LATD1
#define LCD_DATA LATB

void delay_ms(unsigned int ms)
{
    while(ms--) __delay_ms(1);
}

void LCD_Enable()
{
    EN = 1;
    delay_ms(2);
    EN = 0;
    delay_ms(2);
}

void LCD_Command(unsigned char cmd)
{
    RS = 0;
    LCD_DATA = cmd;
    LCD_Enable();
}

void LCD_Char(unsigned char data)
{
    RS = 1;
    LCD_DATA = data;
    LCD_Enable();
}

void LCD_String(const char *msg)
{
    while(*msg) LCD_Char(*msg++);
}

void LCD_SetCursor(unsigned char row, unsigned char col)
{
    if(row == 1)
        LCD_Command(0x80 + col);
    else
        LCD_Command(0xC0 + col);
}

void LCD_Clear()
{
    LCD_Command(0x01);
    delay_ms(10);
}

void LCD_Init()
{
    delay_ms(50);

    LCD_DATA = 0x30; LCD_Enable(); delay_ms(10);
    LCD_DATA = 0x30; LCD_Enable(); delay_ms(10);
    LCD_DATA = 0x30; LCD_Enable(); delay_ms(10);

    LCD_Command(0x38);
    LCD_Command(0x08);
    LCD_Command(0x01);
    LCD_Command(0x06);
    LCD_Command(0x0C);
}

void ADC_Init()
{
    TRISAbits.TRISA1 = 1;
    ANSELAbits.ANSELA1 = 1;

    ADPCH = 0x01;

    ADREFbits.ADPREF = 0;
    ADREFbits.ADNREF = 0;

    ADCLK = 0x3F;
    ADACQ = 0x3F;

    ADCON0bits.FM = 1;
    ADCON0bits.ON = 1;
}

unsigned int ADC_Read()
{
    ADPCH = 0x01;
    delay_ms(1);

    ADCON0bits.GO = 1;
    while(ADCON0bits.GO);

    return ((unsigned int)ADRESH << 8) | ADRESL;
}

void main()
{
    unsigned int adc;
    unsigned int voltage_mv;
    unsigned int last_voltage = 0;
    unsigned int change;

    int accel_x100;

    char line[17];
    const char *state;

    OSCCON1 = 0x60;
    OSCFRQ = 0x02;

    ANSELB = 0x00;
    ANSELD = 0x00;
    TRISB = 0x00;
    TRISD = 0x00;

    LATB = 0x00;
    LATD = 0x00;

    LCD_Init();
    ADC_Init();

    LCD_Clear();

    while(1)
    {
        adc = ADC_Read();

        voltage_mv = ((unsigned long)adc * 5000UL) / 1023UL;

        if(voltage_mv > last_voltage)
            change = voltage_mv - last_voltage;
        else
            change = last_voltage - voltage_mv;

        /*
         * Acceleration calibration:
         * 10.6V = 0.00 m/s2
         * 8.9V or below = -9.81 m/s2, left
         * 12.5V or above = +9.81 m/s2, right
         */

       if(voltage_mv <= 8000)
        {
            accel_x100 = -981;
        }
        else if(voltage_mv >= 12500)
        {
            accel_x100 = 981;
        }
        else if(voltage_mv < 10080)
        {
            accel_x100 = -((long)(10080 - voltage_mv) * 981) / (10080 - 8000);
        }
        else
        {
            accel_x100 = ((long)(voltage_mv - 10080) * 981) / (12500 - 10080);
        }

        if(change > 500)
            state = "Shake";
        else if(voltage_mv >= 11000)
            state = "Tilt_Right";
        else if(voltage_mv <= 9700)
            state = "Tilt_Left";
        else
            state = "Flat";

        LCD_SetCursor(1, 0);
        sprintf(line, "%-16s", state);
        LCD_String(line);

        LCD_SetCursor(2, 0);

        if(accel_x100 < 0)
        {
            sprintf(line, "-%d.%02d m/s2   ",
                    (-accel_x100) / 100,
                    (-accel_x100) % 100);
        }
        else
        {
            sprintf(line, " %d.%02d m/s2   ",
                    accel_x100 / 100,
                    accel_x100 % 100);
        }

        LCD_String(line);

        last_voltage = voltage_mv;

        delay_ms(300);
    }
}
