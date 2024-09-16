/*
 * Gpt_PbCfg.c
 *
 *  Created on: Oct 6, 2023
 *      Author: omar tarek
 */
#include "Gpt.h"

Gpt_ConfigurationType Gpt_Configuration = 
{
    /* Timer 0 */
    .TIM0_mode  = GPT_MODE_OFF,
    .TIM0_clk   = GPT_CLK_PRESCALER_8,
    .TIM0_out   = GPT_OUT_DISCONNECTED,
    /* Timer 1 */
    .TIM1_mode  = GPT_MODE_NORMAL,
    .TIM1_clk   = GPT_CLK_PRESCALER_8,
    .TIM1A_out  = GPT_OUT_DISCONNECTED,
    .TIM1B_out  = GPT_OUT_DISCONNECTED,
    /* Timer 2 */
    .TIM2_mode  = GPT_MODE_OFF,
    .TIM2_clk   = GPT_CLK_OFF,
    .TIM2_out   = GPT_OUT_DISCONNECTED,
};

