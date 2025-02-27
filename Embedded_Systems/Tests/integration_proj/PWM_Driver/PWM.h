#ifndef PWM_DRIVER_H
#define PWM_DRIVER_H

#include <stdint.h>

// PWM frequency in Hz
#define PWM_FREQUENCY 1000

// Define PWM channel numbers
#define PWM_CHANNEL_0 0  // PB6: M0PWM0 (Generator 0)
#define PWM_CHANNEL_1 1  // PB4: M0PWM2 (Generator 1)
#define PWM_CHANNEL_2 2  // PA6: M1PWM2 (Generator 1)

/**
 * @brief Initialize the PWM driver.
 *
 * This function configures the PWM modules, sets up the appropriate GPIO pins,
 * and initializes three PWM channels with the desired frequency.
 */
void PWM_Init(void);

/**
 * @brief Set the duty cycle for a specific PWM channel.
 *
 * The duty cycle is given as a percentage: 0% = LED off, 100% = fully on.
 *
 * @param channel       One of PWM_CHANNEL_0, PWM_CHANNEL_1, or PWM_CHANNEL_2.
 * @param duty_percent  Duty cycle percentage (0 to 100).
 */
void PWM_SetDuty(uint8_t channel, uint8_t duty_percent);

/**
 * @brief Get the current PWM load value.
 *
 * @return uint32_t The load value used to determine the PWM period.
 */
uint32_t PWM_GetLoad(void);

#endif // PWM_DRIVER_H
