/*
 * Icu.c
 *
 *  Created on: Oct 13, 2023
 *      Author: omar tarek
 */
#include "StdTypes.h"
#include "Registers.h"
#include "Macros.h"
#include "Interrupts.h"
#include "Icu.h"

static void (*Callback_Icu) (u16 value) = NULL_PTR;

ISR(VECTOR_TIM1_CAPT)
{
    if (NULL_PTR != Callback_Icu)
    {
        Callback_Icu(ICR1);
    }
}

void Icu_SetTriggerEdge(Icu_EdgeType edge)
{
    if(edge == ICU_EDGE_FALLING)
    {
        CLR_BIT(TCCR1B, 6);
    }
    else if(edge == ICU_EDGE_RISING)
    {
        SET_BIT(TCCR1B, 6);
    }
    else
    {
        /* Do Nothing. */
    }
}

void Icu_EnableInterrupt(void)
{
    SET_BIT(TIMSK, 5);
}

void Icu_DisableInterrupt(void)
{
    CLR_BIT(TIMSK, 5);
}

void Icu_SetCallback(void (*callbackPtr)(u16 value))
{
    Callback_Icu = callbackPtr;
}
