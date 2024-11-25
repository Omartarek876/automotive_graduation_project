/*
 * Pwm.c
 *
 *  Created on: Oct 7, 2023
 *      Author: omar tarek
 */
#include "StdTypes.h"
#include "Macros.h"
#include "Registers.h"
#include "Dio.h"
#include "Pwm.h"

StdReturnType Pwm_Init (Pwm_ConfigurationType* config)
{
    StdReturnType retVal = E_OK;

    if (PWM_MODE_OFF != config->OC0_mode)
    {
        /* Set OC0 Pin to Output */
        Dio_SetPinMode(PWM_PIN_OC0, DIO_MODE_OUTPUT);

        /* Waveform Generation Mode */
        if (config->OC0_mode == PWM_MODE_FAST)
        {
            SET_BIT(TCCR0, 3);
            SET_BIT(TCCR0, 6);
        }
        else if (config->OC0_mode == PWM_MODE_PHASE_CORRECT)
        {
            SET_BIT(TCCR0, 3);
            CLR_BIT(TCCR0, 6);
        }
        else
        {
            retVal = E_NOT_OK;
        }

        /* Compare Match Output Mode */
        if (config->OC0_out == PWM_OUT_CLR_THEN_SET)
        {
            CLR_BIT(TCCR0, 4);
            SET_BIT(TCCR0, 5);
        }
        else if (config->OC0_out == PWM_OUT_SET_THEN_CLR)
        {
            SET_BIT(TCCR0, 4);
            SET_BIT(TCCR0, 5);
        }
        else
        {
            retVal = E_NOT_OK;
        }

        /* Clock Select */
        Pwm_SetClockSource(PWM_CH0, config->OC0_clk);
    }
    if (PWM_MODE_OFF != config->OC1_mode)
    {
        /* Waveform Generation Mode */
        if (config->OC1_mode == PWM_MODE_FAST)
        {
            SET_BIT(TCCR1A, 0);
            SET_BIT(TCCR1A, 1);
            SET_BIT(TCCR1B, 3);
            SET_BIT(TCCR1B, 4);
        }
        else if (config->OC1_mode == PWM_MODE_PHASE_CORRECT)
        {
            SET_BIT(TCCR1A, 0);
            SET_BIT(TCCR1A, 1);
            CLR_BIT(TCCR1B, 3);
            SET_BIT(TCCR1B, 4);
        }
        else if (config->OC1_mode == PWM_MODE_FAST_ICR1)
        {
            CLR_BIT(TCCR1A, 0);
            SET_BIT(TCCR1A, 1);
            SET_BIT(TCCR1B, 3);
            SET_BIT(TCCR1B, 4);
        }
        else if (config->OC1_mode == PWM_MODE_FAST_8_BIT)
        {
            SET_BIT(TCCR1A, 0);
            CLR_BIT(TCCR1A, 1);
            CLR_BIT(TCCR1B, 3);
            CLR_BIT(TCCR1B, 4);
        }
        else if (config->OC1_mode == PWM_MODE_FAST_9_BIT)
        {
            CLR_BIT(TCCR1A, 0);
            SET_BIT(TCCR1A, 1);
            SET_BIT(TCCR1B, 3);
            CLR_BIT(TCCR1B, 4);
        }
        else if (config->OC1_mode == PWM_MODE_FAST_10_BIT)
        {
            SET_BIT(TCCR1A, 0);
            SET_BIT(TCCR1A, 1);
            SET_BIT(TCCR1B, 3);
            CLR_BIT(TCCR1B, 4);
        }
        /** TODO: Complete all PWM1 Modes */
        else
        {
            retVal = E_NOT_OK;
        }

        /* Compare Match Output Mode */
        if (config->OC1A_out == PWM_OUT_CLR_THEN_SET)
        {
            Dio_SetPinMode(PWM_PIN_OC1A, DIO_MODE_OUTPUT);
            CLR_BIT(TCCR1A, 6);
            SET_BIT(TCCR1A, 7);
        }
        else if (config->OC1A_out == PWM_OUT_SET_THEN_CLR)
        {
            Dio_SetPinMode(PWM_PIN_OC1A, DIO_MODE_OUTPUT);
            SET_BIT(TCCR1A, 6);
            SET_BIT(TCCR1A, 7);
        }

        if (config->OC1B_out == PWM_OUT_CLR_THEN_SET)
        {
            Dio_SetPinMode(PWM_PIN_OC1B, DIO_MODE_OUTPUT);
            CLR_BIT(TCCR1A, 4);
            SET_BIT(TCCR1A, 5);
        }
        else if (config->OC1B_out == PWM_OUT_SET_THEN_CLR)
        {
            Dio_SetPinMode(PWM_PIN_OC1B, DIO_MODE_OUTPUT);
            SET_BIT(TCCR1A, 4);
            SET_BIT(TCCR1A, 5);
        }

        /* Clock Select */
        Pwm_SetClockSource(PWM_CH1, config->OC1_clk);
    }
    return retVal;
}

void Pwm_SetClockSource(Pwm_ChannelType channel, Pwm_ClkSourceType clk)
{
    if (PWM_CH0 == channel)
    {
        switch (clk)
        {
        case PWM_CLK_OFF:
            CLR_BIT(TCCR0, 0);
            CLR_BIT(TCCR0, 1);
            CLR_BIT(TCCR0, 2);
            break;
        case PWM_CLK_PRESCALER_1:
            SET_BIT(TCCR0, 0);
            CLR_BIT(TCCR0, 1);
            CLR_BIT(TCCR0, 2);
            break;
        case PWM_CLK_PRESCALER_8:
            CLR_BIT(TCCR0, 0);
            SET_BIT(TCCR0, 1);
            CLR_BIT(TCCR0, 2);
            break;
        case PWM_CLK_PRESCALER_64:
            SET_BIT(TCCR0, 0);
            SET_BIT(TCCR0, 1);
            CLR_BIT(TCCR0, 2);
            break;
        case PWM_CLK_PRESCALER_256:
            CLR_BIT(TCCR0, 0);
            CLR_BIT(TCCR0, 1);
            SET_BIT(TCCR0, 2);
            break;
        case PWM_CLK_PRESCALER_1024:
            SET_BIT(TCCR0, 0);
            CLR_BIT(TCCR0, 1);
            SET_BIT(TCCR0, 2);
            break;
        case PWM_CLK_EXT_FALLING:
            CLR_BIT(TCCR0, 0);
            SET_BIT(TCCR0, 1);
            SET_BIT(TCCR0, 2);
            break;
        case PWM_CLK_EXT_RISING:
            SET_BIT(TCCR0, 0);
            SET_BIT(TCCR0, 1);
            SET_BIT(TCCR0, 2);
            break;
        default:
            break;
        }
    }
    else if (PWM_CH1 == channel)
    {
        switch (clk)
        {
        case PWM_CLK_OFF:
            CLR_BIT(TCCR1B, 0);
            CLR_BIT(TCCR1B, 1);
            CLR_BIT(TCCR1B, 2);
            break;
        case PWM_CLK_PRESCALER_1:
            SET_BIT(TCCR1B, 0);
            CLR_BIT(TCCR1B, 1);
            CLR_BIT(TCCR1B, 2);
            break;
        case PWM_CLK_PRESCALER_8:
            CLR_BIT(TCCR1B, 0);
            SET_BIT(TCCR1B, 1);
            CLR_BIT(TCCR1B, 2);
            break;
        case PWM_CLK_PRESCALER_64:
            SET_BIT(TCCR1B, 0);
            SET_BIT(TCCR1B, 1);
            CLR_BIT(TCCR1B, 2);
            break;
        case PWM_CLK_PRESCALER_256:
            CLR_BIT(TCCR1B, 0);
            CLR_BIT(TCCR1B, 1);
            SET_BIT(TCCR1B, 2);
            break;
        case PWM_CLK_PRESCALER_1024:
            SET_BIT(TCCR1B, 0);
            CLR_BIT(TCCR1B, 1);
            SET_BIT(TCCR1B, 2);
            break;
        case PWM_CLK_EXT_FALLING:
            CLR_BIT(TCCR1B, 0);
            SET_BIT(TCCR1B, 1);
            SET_BIT(TCCR1B, 2);
            break;
        case PWM_CLK_EXT_RISING:
            SET_BIT(TCCR1B, 0);
            SET_BIT(TCCR1B, 1);
            SET_BIT(TCCR1B, 2);
            break;
        default:
            break;
        }
    }
    else if (PWM_CH2 == channel)
    {
        /** TODO: */
    }
    else
    {
        /* Do Nothing */
    }
}

void Pwm_SetTimeOn (Pwm_OutChannelType out, u16 time)
{
    switch (out)
    {
    case PWM_OUT_OC0:
        OCR0 = time;
        break;
    case PWM_OUT_OC1A:
        OCR1A = time;
        break;
    case PWM_OUT_OC1B:
        OCR1B = time;
        break;
    case PWM_OUT_OC2:
        OCR2 = time;
        break;
    default:
        break;
    }
}

void Pwm_SetDutyCycle (Pwm_OutChannelType out, u8 dutyCycle)
{
    switch (out)
    {
    case PWM_OUT_OC0:
        OCR0 = ((u32)dutyCycle*0xFF/100);
        break;
    case PWM_OUT_OC1A:
        OCR1A = ((u32)dutyCycle*0xFFFF/100);
        break;
    case PWM_OUT_OC1B:
        OCR1B = ((u32)dutyCycle*0xFFFF/100);
        break;
    case PWM_OUT_OC2:
        OCR2 = ((u32)dutyCycle*0xFF/100);
        break;
    default:
        break;
    }
}

void Pwm_SetICR1 (u16 value)
{
    ICR1 = value;
}