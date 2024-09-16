/*
 * Gpt.h
 *
 *  Created on: Oct 6, 2023
 *      Author: omar tarek
 */

#ifndef INC_GPT_H_
#define INC_GPT_H_

#include "StdTypes.h"
#include "Dio.h"

#define GPT_PIN_OC0     DIO_PORTB,DIO_PIN3
#define GPT_PIN_OC1A    DIO_PORTD,DIO_PIN5
#define GPT_PIN_OC1B    DIO_PORTD,DIO_PIN4
#define GPT_PIN_OC2     DIO_PORTD,DIO_PIN7
#define GPT_PIN_T0      DIO_PORTB,DIO_PIN0
#define GPT_PIN_T1      DIO_PORTB,DIO_PIN1

typedef enum 
{
    GPT_INT_SOURCE_TIM0_OVF,
    GPT_INT_SOURCE_TIM0_COMP,
    GPT_INT_SOURCE_TIM1_OVF,
    GPT_INT_SOURCE_TIM1_COMPA,
    GPT_INT_SOURCE_TIM1_COMPB,
    GPT_INT_SOURCE_TIM2_OVF,
    GPT_INT_SOURCE_TIM2_COMP
} Gpt_InterruptSourceType;

typedef enum 
{
    GPT_COMP0,
    GPT_COMP1A,
    GPT_COMP1B,
    GPT_COMP2,
} Gpt_CompareChannelType;

typedef enum
{
    GPT_MODE_OFF,
    GPT_MODE_NORMAL,
    GPT_MODE_CTC,
    GPT_MODE_CTC_ICR1
} Gpt_ModeType;

typedef enum
{
    GPT_CLK_OFF,
    GPT_CLK_PRESCALER_1,
    GPT_CLK_PRESCALER_8,
    GPT_CLK_PRESCALER_32,
    GPT_CLK_PRESCALER_64,
    GPT_CLK_PRESCALER_128,
    GPT_CLK_PRESCALER_256,
    GPT_CLK_PRESCALER_1024,
    GPT_CLK_EXT_FALLING,
    GPT_CLK_EXT_RISING
} Gpt_ClkSourceType;

typedef enum
{
    GPT_OUT_DISCONNECTED,
    GPT_OUT_TOGGLE,
    GPT_OUT_CLR,
    GPT_OUT_SET,
} Gpt_OutModeType;

typedef struct 
{
    Gpt_ModeType TIM0_mode;
    Gpt_ClkSourceType TIM0_clk;
    Gpt_OutModeType TIM0_out;
    Gpt_ModeType TIM1_mode;
    Gpt_ClkSourceType TIM1_clk;
    Gpt_OutModeType TIM1A_out;
    Gpt_OutModeType TIM1B_out;
    Gpt_ModeType TIM2_mode;
    Gpt_ClkSourceType TIM2_clk;
    Gpt_OutModeType TIM2_out;
} Gpt_ConfigurationType;

typedef enum 
{
    GPT_CHANNEL_TIM0,
    GPT_CHANNEL_TIM1,
    GPT_CHANNEL_TIM2
} Gpt_ChannelType;

extern Gpt_ConfigurationType Gpt_Configuration;

StdReturnType Gpt_Init(Gpt_ConfigurationType* config);
StdReturnType Gpt_SetClockSource (Gpt_ChannelType channel, Gpt_ClkSourceType clk);
void Gpt_SetCompareValue (Gpt_CompareChannelType channel, u16 value);
void Gpt_SetCounterValue (Gpt_ChannelType channel, u16 value);
u16 Gpt_GetElapsedTime (Gpt_ChannelType channel);
void Gpt_EnableInterrupt(Gpt_InterruptSourceType source);
void Gpt_DisableInterrupt(Gpt_InterruptSourceType source);
void Gpt_SetCallback(Gpt_InterruptSourceType source, void (*callbackPtr) (void));


#endif /* INC_GPT_H_ */
