#include "PWM.h"

/* Global variable to store the PWM load value (same for all channels) */
static uint32 g_pwmLoad = 0;

void PWM_Init(void)
{
    /* Enable PWM modules: PWM0 and PWM1 */
    SYSCTL_RCGCPWM_R |= (1 << 0) | (1 << 1);

    /* Enable PORTA clock */
    SYSCTL_RCGCGPIO_R |= (1 << 0); // Port A

    /* Wait for clock to stabilize */
    volatile uint32 delay = SYSCTL_RCGCGPIO_R;

    /* --- Configure PORTA for PWM1 channels --- */
    // PA6 => M1PWM2, PA7 => M1PWM3
    GPIO_PORTA_AFSEL_R |= (1 << 6) | (1 << 7);
    GPIO_PORTA_PCTL_R &= ~0xFF000000;
    GPIO_PORTA_PCTL_R |= (5 << 24) | (5 << 28);
    GPIO_PORTA_DEN_R |= (1 << 6) | (1 << 7);

    /* Set PWM clock divider to SYSCLK/64 */
    SYSCTL_RCC_R |= (1 << 20);  // USEPWMDIV
    SYSCTL_RCC_R &= ~(0x7 << 17); // Clear PWMDIV bits
    SYSCTL_RCC_R |= (0x7 << 17);  // Set PWMDIV to /64

    /* Calculate load value */
    uint32 pwmClock = 16000000 / 64;              /* 16MHz/64 = 250 kHz */
    g_pwmLoad = (pwmClock / PWM_FREQUENCY) - 1;   /* For 5kHz: load = 50 - 1 = 49 */

    /* --- Initialize PWM1 Generator 1 --- */
    PWM1_1_CTL_R = 0;
    PWM1_1_LOAD_R = g_pwmLoad;
    PWM1_1_CMPA_R = g_pwmLoad;  // PA6
    PWM1_1_CMPB_R = g_pwmLoad;  // PA7
    PWM1_1_GENA_R = 0x0000008C;
    PWM1_1_GENB_R = 0x0000008C;
    PWM1_1_CTL_R |= 0x01;

    /* Enable PWM outputs */
    PWM1_ENABLE_R |= (1 << 2) | (1 << 3);  // M1PWM2 and M1PWM3
}

void PWM_SetDuty(uint8 channel, uint8 duty_percent)
{
    if (duty_percent > 100)
    {
        duty_percent = 100;
    }

    uint32 cmp_value = g_pwmLoad - (((g_pwmLoad + 1) * duty_percent) / 100);

    switch (channel)
    {
        case PWM_CHANNEL_2:  // PA6
            PWM1_1_CMPA_R = cmp_value;
            break;
        case PWM_CHANNEL_3:  // PA7
            PWM1_1_CMPB_R = cmp_value;
            break;
        default:
            /* Invalid or unsupported channel */
            break;
    }
}

uint32 PWM_GetLoad(void)
{
    return g_pwmLoad;
}
