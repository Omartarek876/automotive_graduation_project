#ifndef PWM_DRIVER_H
#define PWM_DRIVER_H

#include "Critical_Files/Platform_Types.h"
#include "Critical_Files/tm4c123gh6pm.h"

/* PWM frequency in Hz */
#define PWM_FREQUENCY 5000

/* Supported PWM channels */
#define PWM_CHANNEL_2 2  /* PA7: M1PWM2 (Generator 1, Output A) */
#define PWM_CHANNEL_3 3  /* PA6: M1PWM3 (Generator 1, Output B) */

/**
 * @brief Initialize the PWM driver (for PA6 and PA7).
 */
void PWM_Init(void);

/**
 * @brief Set the duty cycle for a specific PWM channel (only 2 or 3).
 * @param channel       PWM_CHANNEL_2 or PWM_CHANNEL_3.
 * @param duty_percent  Duty cycle percentage (0 to 100).
 */
void PWM_SetDuty(uint8 channel, uint8 duty_percent);

/**
 * @brief Get the current PWM load value.
 * @return Load value used in PWM timer.
 */
uint32 PWM_GetLoad(void);

#endif /* PWM_DRIVER_H */
