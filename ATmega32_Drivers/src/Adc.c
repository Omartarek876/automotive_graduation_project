/*
 * Adc.c
 *
 *  Created on: Sep 29, 2023
 *      Author: omar tarek
 */
#include "StdTypes.h"
#include "Registers.h"
#include "Macros.h"
#include "Interrupts.h"
#include "Adc.h"

static void (*Callback_Adc) (Adc_ChannelType channel, u16 result) = NULL_PTR;

ISR(VECTOR_ADC)
{
    if (NULL_PTR != Callback_Adc)
    {
        Callback_Adc((ADMUX&0b11111), (ADCDATA&0x3FF));
    }
}

StdReturnType Adc_Init (Adc_ConfigurationType* config)
{
    StdReturnType retVal = E_OK;

    /* Reference Selection Bits */
    if (ADC_REF_AREF == config->reference)
    {
        CLR_BIT(ADMUX, 6);
        CLR_BIT(ADMUX, 7);
    }
    else if (ADC_REF_AVCC == config->reference)
    {
        SET_BIT(ADMUX, 6);
        CLR_BIT(ADMUX, 7);
    }
    else if (ADC_REF_INTERNAL == config->reference)
    {
        SET_BIT(ADMUX, 6);
        SET_BIT(ADMUX, 7);
    }
    else
    {
        retVal = E_NOT_OK;
    }

    /*  ADC Left Adjust Result (OFF) */
    CLR_BIT(ADMUX, 5);

    /* ADC Auto Trigger Enable */
    switch (config->autoTrigger)
    {
    case ADC_AUTO_TRIGGER_OFF:
        CLR_BIT(ADCSRA, 5);
        break;
    case ADC_AUTO_TRIGGER_FREE_RUNNING:
        SET_BIT(ADCSRA, 5);
        CLR_BIT(SFIOR, 5);
        CLR_BIT(SFIOR, 6);
        CLR_BIT(SFIOR, 7);
        break;
    case ADC_AUTO_TRIGGER_ANAL_COMP:
        SET_BIT(ADCSRA, 5);
        SET_BIT(SFIOR, 5);
        CLR_BIT(SFIOR, 6);
        CLR_BIT(SFIOR, 7);
        break;
    case ADC_AUTO_TRIGGER_EXTI0:
        SET_BIT(ADCSRA, 5);
        CLR_BIT(SFIOR, 5);
        SET_BIT(SFIOR, 6);
        CLR_BIT(SFIOR, 7);
        break;
    case ADC_AUTO_TRIGGER_TIM0_COMP:
        /** TODO: Implement the code for this case */
        break;
    case ADC_AUTO_TRIGGER_TIM0_OVF:
        /** TODO: Implement the code for this case */
        break;
    case ADC_AUTO_TRIGGER_TIMB_COMP:
        /** TODO: Implement the code for this case */
        break;
    case ADC_AUTO_TRIGGER_TIM1_OVF:
        /** TODO: Implement the code for this case */
        break;
    case ADC_AUTO_TRIGGER_TIM1_CAP:
        /** TODO: Implement the code for this case */
        break;
    default:
        retVal = E_NOT_OK;
        break;
    }

    /* ADC Prescaler Select Bits */
    switch (config->prescaler)
    {
    case ADC_PRESCALER_2:
        SET_BIT(ADCSRA, 0);
        CLR_BIT(ADCSRA, 1);
        CLR_BIT(ADCSRA, 2);
        break;
    case ADC_PRESCALER_4:
        /** TODO: Implement the code for this case */
        break;
    case ADC_PRESCALER_8:
        /** TODO: Implement the code for this case */
        break;
    case ADC_PRESCALER_16:
        /** TODO: Implement the code for this case */
        break;
    case ADC_PRESCALER_32:
        /** TODO: Implement the code for this case */
        break;
    case ADC_PRESCALER_64:
        /** TODO: Implement the code for this case */
        break;
    case ADC_PRESCALER_128:
        SET_BIT(ADCSRA, 0);
        SET_BIT(ADCSRA, 1);
        SET_BIT(ADCSRA, 2);
        break;
    
    default:
        retVal = E_NOT_OK;
        break;
    }

    /* ADC Enable */
    SET_BIT(ADCSRA, 7);

    return retVal;
}

StdReturnType Adc_StartConversion (Adc_ChannelType channel)
{
    StdReturnType retVal = E_OK;
    if (channel <= 31)
    {
        /* Wait till current conversion completes */
        while (GET_BIT(ADCSRA, 6) == 1);
        /* ADC Channel Select */
        ADMUX &= 0b11100000;
        ADMUX |= channel;
        /* Start new conversion */
        SET_BIT(ADCSRA, 6);
    }
    else
    {
        retVal = E_NOT_OK;
    }
    return retVal;
}

u16 Adc_GetResult (void)
{
    /* Wait till current conversion completes */
    while (GET_BIT(ADCSRA, 4) == 0);
    /* Clear Flag */
    SET_BIT(ADCSRA, 4);
    /* Return ADC result */
    return (ADCDATA & 0x3FF);
}

void Adc_EnableInterrupt(void)
{
    SET_BIT(ADCSRA, 3);
}

void Adc_DisableInterrupt(void)
{
    CLR_BIT(ADCSRA, 3);
}

void Adc_SetCallback(void (*callbackPtr) (Adc_ChannelType channel, u16 result))
{
    Callback_Adc = callbackPtr;
}
