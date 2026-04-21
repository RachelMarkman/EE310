#include <xc.h>
#include <stdint.h>
#include "config_bits.h"
#include "init.h"
#include "functions.h"

volatile uint8_t emergency_flag = 0;

const uint8_t SECRET_DIGIT1 = 3;
const uint8_t SECRET_DIGIT2 = 3;

void __interrupt() ISR(void)
{
    if(PIR0bits.IOCIF)
    {
        if(IOCBFbits.IOCBF2)
        {
            emergency_flag = 1;
            motor_off();
            emergency_melody_ISR();
            IOCBFbits.IOCBF2 = 0;
        }

        PIR0bits.IOCIF = 0;
    }
}

int main(void)
{
    uint8_t digit1, digit2;

    SYSTEM_Init();

    SYS_LED = 1;
    display_blank();
    motor_off();
    buzzer_off();

    while(1)
    {
        if(emergency_flag)
        {
            emergency_flag = 0;
            display_blank();
        }

        digit1 = count_sensor_pulses_pr1();

        if(emergency_flag) continue;

        digit2 = count_sensor_pulses_pr2();

        if(emergency_flag) continue;

        if((digit1 == SECRET_DIGIT1) && (digit2 == SECRET_DIGIT2))
        {
            motor_on();
            delay_ms_block(3000);
            motor_off();
        }
        else
        {
            wrong_code_alert();
        }

        delay_ms_block(250);
        display_blank();
    }

    return 0;
}
