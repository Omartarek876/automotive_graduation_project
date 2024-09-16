/*
 * Pwm_PbCfg.c
 *
 *  Created on: Oct 7, 2023
 *      Author: omar tarek
 */

#include "Pwm.h"

Pwm_ConfigurationType Pwm_Configuration = 
{
    /* Channel OC0 */
    .OC0_mode = PWM_MODE_OFF,
    .OC0_clk  = PWM_CLK_PRESCALER_8,
    .OC0_out  = PWM_OUT_CLR_THEN_SET,
    /* Channel OC1 */
    .OC1_mode = PWM_MODE_FAST_ICR1,
    .OC1_clk  = PWM_CLK_PRESCALER_8,
    .OC1A_out = PWM_OUT_CLR_THEN_SET,
    .OC1B_out = PWM_OUT_DISCONNECTED,
    /* Channel OC2 */
    .OC2_mode = PWM_MODE_OFF,
    .OC2_clk  = PWM_CLK_OFF,
    .OC2_out  = PWM_OUT_DISCONNECTED
};