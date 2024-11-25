/*
 * Pwm.h
 *
 *  Created on: Oct 7, 2023
 *      Author: omar tarek
 */

#ifndef INC_PWM_H_
#define INC_PWM_H_

#include "StdTypes.h"
#include "Dio.h"

#define PWM_PIN_OC0     DIO_PORTB,DIO_PIN3
#define PWM_PIN_OC1A    DIO_PORTD,DIO_PIN5
#define PWM_PIN_OC1B    DIO_PORTD,DIO_PIN4
#define PWM_PIN_OC2     DIO_PORTD,DIO_PIN7
#define PWM_PIN_T0      DIO_PORTB,DIO_PIN0
#define PWM_PIN_T1      DIO_PORTB,DIO_PIN1

typedef enum
{
    PWM_MODE_OFF,
    PWM_MODE_FAST,
    PWM_MODE_PHASE_CORRECT,
    PWM_MODE_PHASE_CORRECT_8_BIT,
    PWM_MODE_PHASE_CORRECT_9_BIT,
    PWM_MODE_PHASE_CORRECT_10_BIT,
    PWM_MODE_PHASE_CORRECT_ICR1,
    PWM_MODE_FAST_8_BIT,
    PWM_MODE_FAST_9_BIT,
    PWM_MODE_FAST_10_BIT,
    PWM_MODE_FAST_ICR1
} Pwm_ModeType;

typedef enum
{
    PWM_CLK_OFF,
    PWM_CLK_PRESCALER_1,
    PWM_CLK_PRESCALER_8,
    PWM_CLK_PRESCALER_32,
    PWM_CLK_PRESCALER_64,
    PWM_CLK_PRESCALER_128,
    PWM_CLK_PRESCALER_256,
    PWM_CLK_PRESCALER_1024,
    PWM_CLK_EXT_FALLING,
    PWM_CLK_EXT_RISING
} Pwm_ClkSourceType;

typedef enum
{
    PWM_OUT_DISCONNECTED,
    PWM_OUT_CLR_THEN_SET,
    PWM_OUT_SET_THEN_CLR
} Pwm_OutModeType;

typedef struct 
{
    Pwm_ModeType OC0_mode;
    Pwm_ClkSourceType OC0_clk;
    Pwm_OutModeType OC0_out;
    Pwm_ModeType OC1_mode;
    Pwm_ClkSourceType OC1_clk;
    Pwm_OutModeType OC1A_out;
    Pwm_OutModeType OC1B_out;
    Pwm_ModeType OC2_mode;
    Pwm_ClkSourceType OC2_clk;
    Pwm_OutModeType OC2_out;
} Pwm_ConfigurationType;

typedef enum
{
    PWM_CH0,
    PWM_CH1,
    PWM_CH2
} Pwm_ChannelType;

typedef enum
{
    PWM_OUT_OC0,
    PWM_OUT_OC1A,
    PWM_OUT_OC1B,
    PWM_OUT_OC2
} Pwm_OutChannelType;

extern Pwm_ConfigurationType Pwm_Configuration;

StdReturnType Pwm_Init (Pwm_ConfigurationType* config);
void Pwm_SetClockSource(Pwm_ChannelType channel, Pwm_ClkSourceType clk);
void Pwm_SetTimeOn (Pwm_OutChannelType out, u16 time);
void Pwm_SetDutyCycle (Pwm_OutChannelType out, u8 dutyCycle);
void Pwm_SetICR1 (u16 value);

#endif /* INC_PWM_H_ */
