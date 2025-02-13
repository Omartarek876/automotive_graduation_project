#include "PWM.h"

// Simple delay function in milliseconds
void delay_ms(uint32 ms)
{
    volatile uint32 i;
    while(ms--)
    {
        for(i = 0; i < 16000; i++) {}  // Roughly 1 ms delay at 16 MHz
    }
}

int main(void)
{
    // (Optional) Update system configuration if necessary
    SYSCTL_RCC_R |= (1 << 22);

    // Initialize the PWM driver (this configures four channels):
    //   PWM_CHANNEL_0: PB6 (M0PWM0, PWM0 Generator 0 Output A)
    //   PWM_CHANNEL_1: PB4 (M0PWM2, PWM0 Generator 1 Output A)
    //   PWM_CHANNEL_2: PA6 (M1PWM2, PWM1 Generator 1 Output A)
    //   PWM_CHANNEL_3: PA7 (M1PWM3, PWM1 Generator 1 Output B)
    PWM_Init();
    uint8 duty;
    while(1)
    {
        // Ramp the brightness quickly from 0% to 100% in 5% increments
        for (duty = 0; duty <= 100; duty += 5)
        {
            PWM_SetDuty(PWM_CHANNEL_0, duty);
            PWM_SetDuty(PWM_CHANNEL_1, duty);
            PWM_SetDuty(PWM_CHANNEL_2, duty);
            PWM_SetDuty(PWM_CHANNEL_3, duty);
            delay_ms(1);  // Very short delay for a fast transition
        }
        // Reset brightness to off
        PWM_SetDuty(PWM_CHANNEL_0, 0);
        PWM_SetDuty(PWM_CHANNEL_1, 0);
        PWM_SetDuty(PWM_CHANNEL_2, 0);
        PWM_SetDuty(PWM_CHANNEL_3, 0);
        delay_ms(10);  // Brief pause before repeating the cycle
    }

}
