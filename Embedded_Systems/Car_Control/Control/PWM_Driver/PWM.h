#ifndef PWM_DRIVER_H
#define PWM_DRIVER_H

#include "Critical_Files/Platform_Types.h"
#include "Critical_Files/tm4c123gh6pm.h"


/* PWM frequency in Hz */
#define PWM_FREQUENCY 5000

/* PWM channel definitions */
#define PWM_CHANNEL_0 0  /* PB6: M0PWM0 (Generator 0, Output A) */
#define PWM_CHANNEL_1 1  /* PB4: M0PWM2 (Generator 1, Output A) */
#define PWM_CHANNEL_2 2  /* PA6: M1PWM2 (Generator 1, Output A) */
#define PWM_CHANNEL_3 3  /* PA7: M1PWM3 (Generator 1, Output B) */

/**
 * @brief Initialize the PWM driver for the four channels.
 *
 * This function configures the PWM modules (PWM0 and PWM1), sets up the appropriate GPIO pins,
 * and initializes four PWM channels with the desired frequency.
 */
void PWM_Init(void);

/**
 * @brief Set the duty cycle for a specific PWM channel.
 *
 * The duty cycle is specified as a percentage (0% = LED/Motor off, 100% = fully on).
 *
 * @param channel       One of PWM_CHANNEL_0, PWM_CHANNEL_1, PWM_CHANNEL_2, or PWM_CHANNEL_3.
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
