/*
 * Gpt.c
 *
 *  Created on: Oct 6, 2023
 *      Author: omar tarek
 */
#include "StdTypes.h"
#include "Macros.h"
#include "Registers.h"
#include "Interrupts.h"
#include "Gpt.h"

static void (*Callback_Gpt_TIM0_OVF) (void) = NULL_PTR;
static void (*Callback_Gpt_TIM0_COMP) (void) = NULL_PTR;
static void (*Callback_Gpt_TIM1_OVF) (void) = NULL_PTR;
static void (*Callback_Gpt_TIM1_COMPA) (void) = NULL_PTR;
static void (*Callback_Gpt_TIM1_COMPB) (void) = NULL_PTR;
static void (*Callback_Gpt_TIM2_OVF) (void) = NULL_PTR;
static void (*Callback_Gpt_TIM2_COMP) (void) = NULL_PTR;

ISR(VECTOR_TIM0_OVF)
{
    if (NULL_PTR != Callback_Gpt_TIM0_OVF)
    {
        Callback_Gpt_TIM0_OVF();
    }
}

ISR(VECTOR_TIM0_COMP)
{
    if (NULL_PTR != Callback_Gpt_TIM0_COMP)
    {
        Callback_Gpt_TIM0_COMP();
    }
}

ISR(VECTOR_TIM1_OVF)
{
    if (NULL_PTR != Callback_Gpt_TIM1_OVF)
    {
        Callback_Gpt_TIM1_OVF();
    }
}

ISR(VECTOR_TIM1_COMPA)
{
    if (NULL_PTR != Callback_Gpt_TIM1_COMPA)
    {
        Callback_Gpt_TIM1_COMPA();
    }
}

ISR(VECTOR_TIM1_COMPB)
{
    if (NULL_PTR != Callback_Gpt_TIM1_COMPB)
    {
        Callback_Gpt_TIM1_COMPB();
    }
}

ISR(VECTOR_TIM2_OVF)
{
    if (NULL_PTR != Callback_Gpt_TIM2_OVF)
    {
        Callback_Gpt_TIM2_OVF();
    }
}

ISR(VECTOR_TIM2_COMP)
{
    if (NULL_PTR != Callback_Gpt_TIM2_COMP)
    {
        Callback_Gpt_TIM2_COMP();
    }
}


StdReturnType Gpt_Init(Gpt_ConfigurationType* config)
{
    StdReturnType retVal = E_OK;
    if (config->TIM0_mode != GPT_MODE_OFF)
    {
        /* Waveform Generation Mode */
        if (GPT_MODE_NORMAL == config->TIM0_mode)
        {
            CLR_BIT(TCCR0, 3);
            CLR_BIT(TCCR0, 6);
        }
        else if (GPT_MODE_CTC == config->TIM0_mode)
        {
            CLR_BIT(TCCR0, 3);
            SET_BIT(TCCR0, 6);
        }
        else
        {
            retVal = E_NOT_OK;
        }

        /*  Compare Output Mode */
        switch (config->TIM0_out)
        {
        case GPT_OUT_DISCONNECTED:
            CLR_BIT(TCCR0, 4);
            CLR_BIT(TCCR0, 5);
            break;
        case GPT_OUT_TOGGLE:
            SET_BIT(TCCR0, 4);
            CLR_BIT(TCCR0, 5);
            break;
        case GPT_OUT_CLR:
            CLR_BIT(TCCR0, 4);
            SET_BIT(TCCR0, 5);
            break;
        case GPT_OUT_SET:
            SET_BIT(TCCR0, 4);
            SET_BIT(TCCR0, 5);
            break;
        default:
            retVal = E_NOT_OK;
            break;
        }

        /* Clock Select */
        retVal = Gpt_SetClockSource(GPT_CHANNEL_TIM0, config->TIM0_clk);
    }

    if (config->TIM1_mode != GPT_MODE_OFF)
    {
        /* Waveform Generation Mode */
        if (GPT_MODE_NORMAL == config->TIM1_mode)
        {
            CLR_BIT(TCCR1A, 0);
            CLR_BIT(TCCR1A, 1);
            CLR_BIT(TCCR1B, 3);
            CLR_BIT(TCCR1B, 4);
        }
        else if (GPT_MODE_CTC == config->TIM1_mode)
        {
            CLR_BIT(TCCR1A, 0);
            CLR_BIT(TCCR1A, 1);
            SET_BIT(TCCR1B, 3);
            CLR_BIT(TCCR1B, 4);
        }
        else if (GPT_MODE_CTC_ICR1 == config->TIM1_mode)
        {
            CLR_BIT(TCCR1A, 0);
            CLR_BIT(TCCR1A, 1);
            SET_BIT(TCCR1B, 3);
            SET_BIT(TCCR1B, 4);
        }
        else
        {
            retVal = E_NOT_OK;
        }

        /*  Compare Output Mode */
        switch (config->TIM1A_out)
        {
        case GPT_OUT_DISCONNECTED:
            CLR_BIT(TCCR1A, 6);
            CLR_BIT(TCCR1A, 7);
            break;
        case GPT_OUT_TOGGLE:
            SET_BIT(TCCR1A, 6);
            CLR_BIT(TCCR1A, 7);
            break;
        case GPT_OUT_CLR:
            CLR_BIT(TCCR1A, 6);
            SET_BIT(TCCR1A, 7);
            break;
        case GPT_OUT_SET:
            SET_BIT(TCCR1A, 6);
            SET_BIT(TCCR1A, 7);
            break;
        default:
            retVal = E_NOT_OK;
            break;
        }
        switch (config->TIM1B_out)
        {
        case GPT_OUT_DISCONNECTED:
            CLR_BIT(TCCR1A, 4);
            CLR_BIT(TCCR1A, 5);
            break;
        case GPT_OUT_TOGGLE:
            SET_BIT(TCCR1A, 4);
            CLR_BIT(TCCR1A, 5);
            break;
        case GPT_OUT_CLR:
            CLR_BIT(TCCR1A, 4);
            SET_BIT(TCCR1A, 5);
            break;
        case GPT_OUT_SET:
            SET_BIT(TCCR1A, 4);
            SET_BIT(TCCR1A, 5);
            break;
        default:
            retVal = E_NOT_OK;
            break;
        }

        /* Clock Select */
        retVal = Gpt_SetClockSource(GPT_CHANNEL_TIM1, config->TIM1_clk);
    }

    if (config->TIM2_mode != GPT_MODE_OFF)
    {
        /* Waveform Generation Mode */
        if (GPT_MODE_NORMAL == config->TIM2_mode)
        {
            CLR_BIT(TCCR2, 3);
            CLR_BIT(TCCR2, 6);
        }
        else if (GPT_MODE_CTC == config->TIM2_mode)
        {
            CLR_BIT(TCCR2, 3);
            SET_BIT(TCCR2, 6);
        }
        else
        {
            retVal = E_NOT_OK;
        }

        /*  Compare Output Mode */
        switch (config->TIM2_out)
        {
        case GPT_OUT_DISCONNECTED:
            CLR_BIT(TCCR2, 4);
            CLR_BIT(TCCR2, 5);
            break;
        case GPT_OUT_TOGGLE:
            SET_BIT(TCCR2, 4);
            CLR_BIT(TCCR2, 5);
            break;
        case GPT_OUT_CLR:
            CLR_BIT(TCCR2, 4);
            SET_BIT(TCCR2, 5);
            break;
        case GPT_OUT_SET:
            SET_BIT(TCCR2, 4);
            SET_BIT(TCCR2, 5);
            break;
        default:
            retVal = E_NOT_OK;
            break;
        }
        /* Clock Select */
        retVal = Gpt_SetClockSource(GPT_CHANNEL_TIM2, config->TIM2_clk);
    }

    return retVal;
}

StdReturnType Gpt_SetClockSource (Gpt_ChannelType channel, Gpt_ClkSourceType clk)
{
    StdReturnType retVal = E_OK;
    if (GPT_CHANNEL_TIM0 == channel)
    {
        switch (clk)
        {
        case GPT_CLK_OFF:
            CLR_BIT(TCCR0, 0);
            CLR_BIT(TCCR0, 1);
            CLR_BIT(TCCR0, 2);
            break;
        case GPT_CLK_PRESCALER_1:
            SET_BIT(TCCR0, 0);
            CLR_BIT(TCCR0, 1);
            CLR_BIT(TCCR0, 2);
            break;
        case GPT_CLK_PRESCALER_8:
            CLR_BIT(TCCR0, 0);
            SET_BIT(TCCR0, 1);
            CLR_BIT(TCCR0, 2);
            break;
        case GPT_CLK_PRESCALER_64:
            SET_BIT(TCCR0, 0);
            SET_BIT(TCCR0, 1);
            CLR_BIT(TCCR0, 2);
            break;
        case GPT_CLK_PRESCALER_256:
            CLR_BIT(TCCR0, 0);
            CLR_BIT(TCCR0, 1);
            SET_BIT(TCCR0, 2);
            break;
        case GPT_CLK_PRESCALER_1024:
            SET_BIT(TCCR0, 0);
            CLR_BIT(TCCR0, 1);
            SET_BIT(TCCR0, 2);
            break;
        case GPT_CLK_EXT_FALLING:
            CLR_BIT(TCCR0, 0);
            SET_BIT(TCCR0, 1);
            SET_BIT(TCCR0, 2);
            break;
        case GPT_CLK_EXT_RISING:
            SET_BIT(TCCR0, 0);
            SET_BIT(TCCR0, 1);
            SET_BIT(TCCR0, 2);
            break;
        default:
            retVal = E_NOT_OK;
            break;
        }
    }
    else if (GPT_CHANNEL_TIM1 == channel)
    {
        switch (clk)
        {
        case GPT_CLK_OFF:
            CLR_BIT(TCCR1B, 0);
            CLR_BIT(TCCR1B, 1);
            CLR_BIT(TCCR1B, 2);
            break;
        case GPT_CLK_PRESCALER_1:
            SET_BIT(TCCR1B, 0);
            CLR_BIT(TCCR1B, 1);
            CLR_BIT(TCCR1B, 2);
            break;
        case GPT_CLK_PRESCALER_8:
            CLR_BIT(TCCR1B, 0);
            SET_BIT(TCCR1B, 1);
            CLR_BIT(TCCR1B, 2);
            break;
        case GPT_CLK_PRESCALER_64:
            SET_BIT(TCCR1B, 0);
            SET_BIT(TCCR1B, 1);
            CLR_BIT(TCCR1B, 2);
            break;
        case GPT_CLK_PRESCALER_256:
            CLR_BIT(TCCR1B, 0);
            CLR_BIT(TCCR1B, 1);
            SET_BIT(TCCR1B, 2);
            break;
        case GPT_CLK_PRESCALER_1024:
            SET_BIT(TCCR1B, 0);
            CLR_BIT(TCCR1B, 1);
            SET_BIT(TCCR1B, 2);
            break;
        case GPT_CLK_EXT_FALLING:
            CLR_BIT(TCCR1B, 0);
            SET_BIT(TCCR1B, 1);
            SET_BIT(TCCR1B, 2);
            break;
        case GPT_CLK_EXT_RISING:
            SET_BIT(TCCR1B, 0);
            SET_BIT(TCCR1B, 1);
            SET_BIT(TCCR1B, 2);
            break;
        default:
            retVal = E_NOT_OK;
            break;
        }
    }
    else if (GPT_CHANNEL_TIM2 == channel)
    {
        switch (clk)
        {
        case GPT_CLK_OFF:
            CLR_BIT(TCCR2, 0);
            CLR_BIT(TCCR2, 1);
            CLR_BIT(TCCR2, 2);
            break;
        case GPT_CLK_PRESCALER_1:
            SET_BIT(TCCR2, 0);
            CLR_BIT(TCCR2, 1);
            CLR_BIT(TCCR2, 2);
            break;
        case GPT_CLK_PRESCALER_8:
            CLR_BIT(TCCR2, 0);
            SET_BIT(TCCR2, 1);
            CLR_BIT(TCCR2, 2);
            break;
        case GPT_CLK_PRESCALER_32:
            SET_BIT(TCCR2, 0);
            SET_BIT(TCCR2, 1);
            CLR_BIT(TCCR2, 2);
            break;
        case GPT_CLK_PRESCALER_64:
            CLR_BIT(TCCR2, 0);
            CLR_BIT(TCCR2, 1);
            SET_BIT(TCCR2, 2);
            break;
        case GPT_CLK_PRESCALER_128:
            SET_BIT(TCCR2, 0);
            CLR_BIT(TCCR2, 1);
            SET_BIT(TCCR2, 2);
            break;
        case GPT_CLK_PRESCALER_256:
            CLR_BIT(TCCR2, 0);
            SET_BIT(TCCR2, 1);
            SET_BIT(TCCR2, 2);
            break;
        case GPT_CLK_PRESCALER_1024:
            SET_BIT(TCCR2, 0);
            SET_BIT(TCCR2, 1);
            SET_BIT(TCCR2, 2);
            break;
        default:
            retVal = E_NOT_OK;
            break;
        }
    }
    else
    {
        retVal = E_NOT_OK;
    }
    return retVal;
}
void Gpt_SetCompareValue (Gpt_CompareChannelType channel, u16 value)
{
    switch (channel)
    {
    case GPT_COMP0:
        OCR0 = value;
        break;
    case GPT_COMP1A:
        OCR1A = value;
        break;
    case GPT_COMP1B:
        OCR1B = value;
        break;
    case GPT_COMP2:
        OCR2 = value;
        break;
    default:
        break;
    }
}
void Gpt_SetCounterValue (Gpt_ChannelType channel, u16 value)
{
    switch (channel)
    {
    case GPT_CHANNEL_TIM0:
        TCNT0 = value;
        break;
    case GPT_CHANNEL_TIM1:
        TCNT1 = value;
        break;
    case GPT_CHANNEL_TIM2:
        TCNT2 = value;
        break;
    default:
        break;
    }
}

u16 Gpt_GetElapsedTime (Gpt_ChannelType channel)
{
    u16 value = 0;
    if (GPT_CHANNEL_TIM0 == channel)
    {
        value = TCNT0;
    }
    else if (GPT_CHANNEL_TIM1 == channel)
    {
        value = TCNT1;
    }
    else if (GPT_CHANNEL_TIM2 == channel)
    {
        value = TCNT2;
    }
    return value;
}

void Gpt_EnableInterrupt(Gpt_InterruptSourceType source)
{
    switch (source)
    {
    case GPT_INT_SOURCE_TIM0_OVF:
        SET_BIT(TIMSK, 0);
        break;
    case GPT_INT_SOURCE_TIM0_COMP:
        SET_BIT(TIMSK, 1);
        break;
    case GPT_INT_SOURCE_TIM1_OVF:
        SET_BIT(TIMSK, 2);
        break;
    case GPT_INT_SOURCE_TIM1_COMPA:
        SET_BIT(TIMSK, 4);
        break;
    case GPT_INT_SOURCE_TIM1_COMPB:
        SET_BIT(TIMSK, 3);
        break;
    case GPT_INT_SOURCE_TIM2_OVF:
        SET_BIT(TIMSK, 6);
        break;
    case GPT_INT_SOURCE_TIM2_COMP:
        SET_BIT(TIMSK, 7);
        break;
    default:
        break;
    }
}

void Gpt_DisableInterrupt(Gpt_InterruptSourceType source)
{
    switch (source)
    {
    case GPT_INT_SOURCE_TIM0_OVF:
        CLR_BIT(TIMSK, 0);
        break;
    case GPT_INT_SOURCE_TIM0_COMP:
        CLR_BIT(TIMSK, 1);
        break;
    case GPT_INT_SOURCE_TIM1_OVF:
        CLR_BIT(TIMSK, 2);
        break;
    case GPT_INT_SOURCE_TIM1_COMPA:
        CLR_BIT(TIMSK, 4);
        break;
    case GPT_INT_SOURCE_TIM1_COMPB:
        CLR_BIT(TIMSK, 3);
        break;
    case GPT_INT_SOURCE_TIM2_OVF:
        CLR_BIT(TIMSK, 6);
        break;
    case GPT_INT_SOURCE_TIM2_COMP:
        CLR_BIT(TIMSK, 7);
        break;
    default:
        break;
    }
}

void Gpt_SetCallback(Gpt_InterruptSourceType source, void (*callbackPtr) (void))
{
    switch (source)
    {
    case GPT_INT_SOURCE_TIM0_OVF:
        Callback_Gpt_TIM0_OVF = callbackPtr;
        break;
    case GPT_INT_SOURCE_TIM0_COMP:
        Callback_Gpt_TIM0_COMP = callbackPtr;
        break;
    case GPT_INT_SOURCE_TIM1_OVF:
        Callback_Gpt_TIM1_OVF = callbackPtr;
        break;
    case GPT_INT_SOURCE_TIM1_COMPA:
        Callback_Gpt_TIM1_COMPA = callbackPtr;
        break;
    case GPT_INT_SOURCE_TIM1_COMPB:
        Callback_Gpt_TIM1_COMPB = callbackPtr;
        break;
    case GPT_INT_SOURCE_TIM2_OVF:
        Callback_Gpt_TIM2_OVF = callbackPtr;
        break;
    case GPT_INT_SOURCE_TIM2_COMP:
        Callback_Gpt_TIM2_COMP = callbackPtr;
        break;
    default:
        break;
    }
}
