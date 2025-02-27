#include "PWM.h"

/* Global variable to store the PWM load value (same for all channels) */
static uint32 g_pwmLoad = 0;

void PWM_Init(void)
{
    /* (Optional) Update system clock settings if needed */
    SYSCTL_RCC_R |= (1 << 22);

    /* Enable PWM modules: PWM0 and PWM1 */
    SYSCTL_RCGCPWM_R |= (1 << 0) | (1 << 1);

    /* --- Configure PORTB for PWM0 channels --- */
    /* PB4 will be used for M0PWM2 (PWM_CHANNEL_1) */
    /* PB6 will be used for M0PWM0 (PWM_CHANNEL_0) */
    GPIO_PORTB_AFSEL_R |= (1 << 4) | (1 << 6);
    GPIO_PORTB_PCTL_R &= ~0x0F0F0000;             /* Clear PCTL bits for PB4 & PB6 */
    GPIO_PORTB_PCTL_R |= (4 << 16) | (4 << 24);     /* Set function 4 (PWM) */
    GPIO_PORTB_DEN_R |= (1 << 4) | (1 << 6);

    /* --- Configure PORTA for PWM1 channels --- */
    /* PA6 will be used for M1PWM2 (PWM_CHANNEL_2) */
    /* PA7 will be used for M1PWM3 (PWM_CHANNEL_3) */
    GPIO_PORTA_AFSEL_R |= (1 << 6) | (1 << 7);
    GPIO_PORTA_PCTL_R &= ~0xFF000000;              /* Clear PCTL bits for PA6 & PA7 */
    GPIO_PORTA_PCTL_R |= (5 << 24) | (5 << 28);      /* Set function 5 (PWM) */
    GPIO_PORTA_DEN_R |= (1 << 6) | (1 << 7);

    /* Set PWM clock divider to SYSCLK/64 */
    SYSCTL_RCC_R |= (1 << 20);

    {
        uint32 pwmClock = 16000000 / 64;              /* 16MHz/64 = 250 kHz */
        g_pwmLoad = (pwmClock / PWM_FREQUENCY) - 1;     /* For 5kHz: load = 50 - 1 = 49 */
    }

    /* --- Initialize PWM0, Generator 0 for PWM_CHANNEL_0 (PB6: M0PWM0) --- */
    PWM0_0_CTL_R = 0;              /* Disable Generator 0 */
    PWM0_0_LOAD_R = g_pwmLoad;
    PWM0_0_CMPA_R = g_pwmLoad;     /* Start with LED/Motor off */
    PWM0_0_GENA_R = 0x0000008C;    /* Output HIGH until counter reaches CMPA then LOW */
    PWM0_0_CTL_R |= 0x01;          /* Enable Generator 0 */
    PWM0_ENABLE_R |= (1 << 0);     /* Enable PWM output on channel 0 */

    /* --- Initialize PWM0, Generator 1 for PWM_CHANNEL_1 (PB4: M0PWM2) --- */
    PWM0_1_CTL_R = 0;
    PWM0_1_LOAD_R = g_pwmLoad;
    PWM0_1_CMPA_R = g_pwmLoad;
    PWM0_1_GENA_R = 0x0000008C;
    PWM0_1_CTL_R |= 0x01;
    PWM0_ENABLE_R |= (1 << 2);     /* Enable PWM output on channel 1 */

    /* --- Initialize PWM1, Generator 1 for PWM_CHANNEL_2 and PWM_CHANNEL_3 (PA6 and PA7) --- */
    PWM1_1_CTL_R = 0;
    PWM1_1_LOAD_R = g_pwmLoad;
    PWM1_1_CMPA_R = g_pwmLoad;     /* For PA6 (Output A) */
    PWM1_1_CMPB_R = g_pwmLoad;     /* For PA7 (Output B) */
    PWM1_1_GENA_R = 0x0000008C;    /* Configure action for output A */
    PWM1_1_GENB_R = 0x0000008C;    /* Configure action for output B */
    PWM1_1_CTL_R |= 0x01;
    PWM1_ENABLE_R |= (1 << 2) | (1 << 3);  /* Enable outputs on channels 2 and 3 */
}

void PWM_SetDuty(uint8 channel, uint8 duty_percent)
{
    if(duty_percent > 100)
    {
        duty_percent = 100;
    }
    /* In our non-inverted configuration:
     * CMPA = g_pwmLoad => output off,
     * CMPA = 0         => full on.
     * Calculate CMPA such that:
     * duty_percent = 0   --> CMPA = g_pwmLoad
     * duty_percent = 100 --> CMPA = 0
     */
    {
        uint32 cmp_value = g_pwmLoad - (((g_pwmLoad + 1) * duty_percent) / 100); // 49 - 5 = 44 // 49 - 45 = 4
        switch(channel)
        {
            case PWM_CHANNEL_0:
                PWM0_0_CMPA_R = cmp_value;
                break;
            case PWM_CHANNEL_1:
                PWM0_1_CMPA_R = cmp_value;
                break;
            case PWM_CHANNEL_2:
                PWM1_1_CMPA_R = cmp_value;
                break;
            case PWM_CHANNEL_3:
                PWM1_1_CMPB_R = cmp_value;
                break;
            default:
                /* Invalid channel: do nothing */
                break;
        }
    }
}

uint32 PWM_GetLoad(void)
{
    return g_pwmLoad;
}
