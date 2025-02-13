#include "PWM.h"
#include "tm4c123gh6pm.h"

// Global variable to store the PWM load value (same for all channels)
static uint32_t g_pwmLoad = 0;

void PWM_Init(void)
{
    // Enable PWM modules: PWM0 and PWM1
    SYSCTL_RCGCPWM_R |= (1 << 0) | (1 << 1);

    // Enable GPIO ports: PORTB (for PB4 and PB6) and PORTA (for PA6)
    SYSCTL_RCGCGPIO_R |= (1 << 1) | (1 << 0);
    while ((SYSCTL_PRGPIO_R & ((1 << 1) | (1 << 0))) != ((1 << 1) | (1 << 0)))
        ; // wait for PORTA and PORTB to be ready

    // --- Configure PORTB for PWM0 channels ---
    // PB4 will be used for M0PWM2 (PWM_CHANNEL_1)
    // PB6 will be used for M0PWM0 (PWM_CHANNEL_0)
    GPIO_PORTB_AFSEL_R |= (1 << 4) | (1 << 6);
    GPIO_PORTB_PCTL_R &= ~0x0F0F0000;             // Clear PCTL bits for PB4 and PB6
    GPIO_PORTB_PCTL_R |= (4 << 16) | (4 << 24);     // Set function 4 for PWM on PB4 and PB6
    GPIO_PORTB_DEN_R |= (1 << 4) | (1 << 6);

    // --- Configure PORTA for PWM1 channel ---
    // PA6 will be used for M1PWM2 (PWM_CHANNEL_2)
    GPIO_PORTA_AFSEL_R |= (1 << 6);
    GPIO_PORTA_PCTL_R &= ~0x0F000000;             // Clear PCTL bits for PA6
    GPIO_PORTA_PCTL_R |= (5 << 24);                // Set function 5 for PWM on PA6
    GPIO_PORTA_DEN_R |= (1 << 6);

    // Set PWM clock divider to SYSCLK/64
    SYSCTL_RCC_R |= (1 << 20);

    // Calculate PWM load value based on a 16MHz clock divided by 64:
    // pwmClock = 16MHz / 64 = 250 kHz, so for PWM_FREQUENCY (1 kHz):
    // load = (250000 / 1000) - 1 = 250 - 1 = 249
    uint32_t pwmClock = 16000000 / 64;
    g_pwmLoad = (pwmClock / PWM_FREQUENCY) - 1;

    // --- Initialize PWM0, Generator 0 for PWM_CHANNEL_0 (PB6) ---
    PWM0_0_CTL_R = 0;              // Disable generator 0 during configuration
    PWM0_0_LOAD_R = g_pwmLoad;
    PWM0_0_CMPA_R = g_pwmLoad;     // Start with LED off (CMPA = load)
    PWM0_0_GENA_R = 0x0000008C;    // Non-inverted PWM: output stays HIGH until counter reaches CMPA, then goes LOW
    PWM0_0_CTL_R |= 0x01;          // Enable generator 0
    PWM0_ENABLE_R |= (1 << 0);     // Enable PWM output on channel 0

    // --- Initialize PWM0, Generator 1 for PWM_CHANNEL_1 (PB4) ---
    PWM0_1_CTL_R = 0;
    PWM0_1_LOAD_R = g_pwmLoad;
    PWM0_1_CMPA_R = g_pwmLoad;
    PWM0_1_GENA_R = 0x0000008C;
    PWM0_1_CTL_R |= 0x01;
    PWM0_ENABLE_R |= (1 << 2);     // Enable PWM output on channel 1

    // --- Initialize PWM1, Generator 1 for PWM_CHANNEL_2 (PA6) ---
    PWM1_1_CTL_R = 0;
    PWM1_1_LOAD_R = g_pwmLoad;
    PWM1_1_CMPA_R = g_pwmLoad;
    PWM1_1_GENA_R = 0x0000008C;
    PWM1_1_CTL_R |= 0x01;
    PWM1_ENABLE_R |= (1 << 2);     // Enable PWM output on channel 2
}

void PWM_SetDuty(uint8_t channel, uint8_t duty_percent)
{
    if (duty_percent > 100)
        duty_percent = 100;

    // In our PWM configuration (non-inverted mode):
    // - CMPA = g_pwmLoad -> LED is off (on-time = 0)
    // - CMPA = 0 -> LED is fully on (on-time = g_pwmLoad)
    // Calculate CMPA value such that:
    // duty_percent = 0 -> CMPA = g_pwmLoad
    // duty_percent = 100 -> CMPA = 0
    uint32_t cmp_value = g_pwmLoad - (((g_pwmLoad + 1) * duty_percent) / 100);

    switch (channel)
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
        default:
            // Invalid channel – do nothing
            break;
    }
}

uint32_t PWM_GetLoad(void)
{
    return g_pwmLoad;
}
