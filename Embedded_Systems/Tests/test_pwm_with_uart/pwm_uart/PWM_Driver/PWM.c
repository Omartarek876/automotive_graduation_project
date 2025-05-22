#include "PWM.h"

/* Global variable to store the PWM load value (same for all channels) */
static uint32 g_pwmLoad = 0;

void PWM_Init(void)
{
    /* ✅ Set PWM clock divider to SYSCLK / 64 without affecting other peripherals */
    SYSCTL_RCC_R = (SYSCTL_RCC_R & ~(0x000E0000)) | (0x5 << 17); // Divisor = 64
    SYSCTL_RCC_R |= (1 << 20); // Enable use of PWM divider

    /* ✅ Enable PWM modules: PWM0 and PWM1 */
    SYSCTL_RCGCPWM_R |= (1 << 0) | (1 << 1);

    /* ✅ Enable GPIO Port A clock for PWM1 output pins PA6, PA7 */
    SYSCTL_RCGCGPIO_R |= (1 << 0); // Port A
    while ((SYSCTL_PRGPIO_R & (1 << 0)) == 0); // Wait for Port A to be ready

    /* ✅ Configure PA6 and PA7 for PWM (M1PWM2 and M1PWM3) */
    GPIO_PORTA_AFSEL_R |= (1 << 6) | (1 << 7);              // Alternate function
    GPIO_PORTA_PCTL_R &= ~0xFF000000;                       // Clear bits
    GPIO_PORTA_PCTL_R |= (5 << 24) | (5 << 28);             // Set PWM function
    GPIO_PORTA_DEN_R |= (1 << 6) | (1 << 7);                // Digital enable
    GPIO_PORTA_AMSEL_R &= ~((1 << 6) | (1 << 7));           // Disable analog

    /* ✅ Calculate and set the PWM load value */
    {
        uint32 pwmClock = 16000000 / 64;              // 16 MHz / 64 = 250 kHz
        g_pwmLoad = (pwmClock / PWM_FREQUENCY) - 1;   // For 5kHz PWM: 250000 / 5000 - 1 = 49
    }

    /* ✅ Configure PWM1 Generator 1 for PA6 (PWM_CHANNEL_2) and PA7 (PWM_CHANNEL_3) */
    PWM1_1_CTL_R = 0;                    // Disable generator during config
    PWM1_1_LOAD_R = g_pwmLoad;
    PWM1_1_CMPA_R = g_pwmLoad;          // Initial duty cycle = 0% (CMP = LOAD)
    PWM1_1_CMPB_R = g_pwmLoad;
    PWM1_1_GENA_R = 0x0000008C;         // PWM: Set on LOAD, clear on CMPA
    PWM1_1_GENB_R = 0x0000008C;         // PWM: Set on LOAD, clear on CMPB
    PWM1_1_CTL_R |= 0x01;               // Enable generator
    PWM1_ENABLE_R |= (1 << 2) | (1 << 3); // Enable PWM1 outputs 2 and 3
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
        case PWM_CHANNEL_0:
            // PWM0_0_CMPA_R = cmp_value; // PB6
            break;
        case PWM_CHANNEL_1:
            // PWM0_1_CMPA_R = cmp_value; // PB4
            break;
        case PWM_CHANNEL_2:
            PWM1_1_CMPA_R = cmp_value;   // PA6
            break;
        case PWM_CHANNEL_3:
            PWM1_1_CMPB_R = cmp_value;   // PA7
            break;
        default:
            // Invalid channel
            break;
    }
}

uint32 PWM_GetLoad(void)
{
    return g_pwmLoad;
}
