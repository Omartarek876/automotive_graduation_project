#ifndef PWM_DRIVER_H
#define PWM_DRIVER_H

#include "Critical_Files/Platform_Types.h"
#include "Critical_Files/tm4c123gh6pm.h"

/* PWM frequency in Hz */
#define PWM_FREQUENCY 5000  /* Increased from 1 kHz to 5 kHz */

/* PWM channel definitions */
#define PWM_CHANNEL_0 0  /* PB6: M0PWM0 (Generator 0, Output A) */

/**
 * @brief Initialize the PWM driver for the required channel.
 *
 * This function configures the PWM module (PWM0), sets up the appropriate GPIO pin,
 * and initializes the PWM channel with the desired frequency.
 */
void PWM_Init(void);

/**
 * @brief Set the duty cycle for a specific PWM channel.
 *
 * The duty cycle is specified as a percentage (0% = Motor off, 100% = fully on).
 *
 * @param channel       PWM_CHANNEL_0.
 * @param duty_percent  Duty cycle percentage (0 to 100).
 */
void PWM_SetDuty(uint8 channel, uint8 duty_percent);

/**
 * @brief Get the current PWM load value.
 *
 * @return uint32 The load value used to determine the PWM period.
 */
uint32 PWM_GetLoad(void);

#endif /* PWM_DRIVER_H */