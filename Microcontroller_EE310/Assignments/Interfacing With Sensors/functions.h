#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <xc.h>
#include <stdint.h>

#define PR1_PORT    PORTBbits.RB0
#define PR2_PORT    PORTBbits.RB1

#define SYS_LED     LATAbits.LATA0
#define RELAY_OUT   LATAbits.LATA1
#define BUZZER_OUT  LATAbits.LATA2

void delay_ms_block(uint16_t ms)
{
    while(ms--)
    {
        __delay_ms(1);
    }
}

void display_digit(uint8_t digit)
{
    switch(digit)
    {
        case 0: LATD = 0b00111111; break;
        case 1: LATD = 0b00000110; break;
        case 2: LATD = 0b01011011; break;
        case 3: LATD = 0b01001111; break;
        case 4: LATD = 0b01100110; break;
        case 5: LATD = 0b01101101; break;
        case 6: LATD = 0b01111101; break;
        case 7: LATD = 0b00000111; break;
        case 8: LATD = 0b01111111; break;
        case 9: LATD = 0b01101111; break;
        default: LATD = 0x00; break;
    }
}

void display_blank(void)
{
    LATD = 0x00;
}

void motor_on(void)
{
    RELAY_OUT = 1;
}

void motor_off(void)
{
    RELAY_OUT = 0;
}

void buzzer_on(void)
{
    BUZZER_OUT = 1;
}

void buzzer_off(void)
{
    BUZZER_OUT = 0;
}

void wrong_code_alert(void)
{
    buzzer_on();
    delay_ms_block(2000);
    buzzer_off();
}

void emergency_melody_ISR(void)
{
    uint8_t i;

    for(i = 0; i < 4; i++)
    {
        buzzer_on();
        delay_ms_block(150);
        buzzer_off();
        delay_ms_block(100);
    }

    for(i = 0; i < 2; i++)
    {
        buzzer_on();
        delay_ms_block(400);
        buzzer_off();
        delay_ms_block(150);
    }
}

uint8_t count_sensor_pulses_pr1(void)
{
    uint8_t count = 0;
    uint16_t idle_ms = 0;
    uint8_t prev_state = 0;

    while(PR1_PORT == 0);

    while(1)
    {
        uint8_t current_state = PR1_PORT;

        if((current_state == 1) && (prev_state == 0))
        {
            count++;
            if(count > 9)
            {
                count = 0;
            }

            display_digit(count);
            idle_ms = 0;
            delay_ms_block(25);
        }
        else
        {
            delay_ms_block(10);

            if(count > 0)
            {
                idle_ms += 10;
                if(idle_ms >= 700)
                {
                    break;
                }
            }
        }

        prev_state = current_state;
    }

    return count;
}

uint8_t count_sensor_pulses_pr2(void)
{
    uint8_t count = 0;
    uint16_t idle_ms = 0;
    uint8_t prev_state = 0;

    while(PR2_PORT == 0);

    while(1)
    {
        uint8_t current_state = PR2_PORT;

        if((current_state == 1) && (prev_state == 0))
        {
            count++;
            if(count > 9)
            {
                count = 0;
            }

            display_digit(count);
            idle_ms = 0;
            delay_ms_block(25);
        }
        else
        {
            delay_ms_block(10);

            if(count > 0)
            {
                idle_ms += 10;
                if(idle_ms >= 700)
                {
                    break;
                }
            }
        }

        prev_state = current_state;
    }

    return count;
}

#endif
