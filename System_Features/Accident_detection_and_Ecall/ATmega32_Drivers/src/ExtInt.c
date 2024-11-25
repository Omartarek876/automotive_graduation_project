/*
 * ExtInt.c
 *
 *  Created on: Sep 23, 2023
 *      Author: omar tarek
 */
#include "StdTypes.h"
#include "Macros.h"
#include "Registers.h"
#include "Interrupts.h"
#include "ExtInt.h"

static void (*Callbck_Int0) (void) = NULL_PTR;
static void (*Callbck_Int1) (void) = NULL_PTR;
static void (*Callbck_Int2) (void) = NULL_PTR;

ISR(VECTOR_INT0)
{
    if(NULL_PTR != Callbck_Int0)
    {
        Callbck_Int0();
    }
}

ISR(VECTOR_INT1)
{
    if(NULL_PTR != Callbck_Int1)
    {
        Callbck_Int1();
    }
}

ISR(VECTOR_INT2)
{
    if(NULL_PTR != Callbck_Int2)
    {
        Callbck_Int2();
    }
}

StdReturnType ExtInt_SetSenseControl (ExtInt_SourceType source, ExtInt_SenseType sense)
{
    StdReturnType retVal = E_OK;
    if (EXTINT_SOURCE_INT0 == source)
    {
        switch (sense)
        {
        case EXTINT_SENSE_LOW_LEVEL:
            CLR_BIT(MCUCR, 0);
            CLR_BIT(MCUCR, 1);
            break;
        case EXTINT_SENSE_ON_CHANGE:
            SET_BIT(MCUCR, 0);
            CLR_BIT(MCUCR, 1);
            break;
        case EXTINT_SENSE_FALLING_EDGE:
            CLR_BIT(MCUCR, 0);
            SET_BIT(MCUCR, 1);
            break;
        case EXTINT_SENSE_RISING_EDGE:
            SET_BIT(MCUCR, 0);
            SET_BIT(MCUCR, 1);
            break;
        default:
            retVal = E_NOT_OK;
            break;
        }
    }
    else if (EXTINT_SOURCE_INT1 == source)
    {
        switch (sense)
        {
        case EXTINT_SENSE_LOW_LEVEL:
            CLR_BIT(MCUCR, 2);
            CLR_BIT(MCUCR, 3);
            break;
        case EXTINT_SENSE_ON_CHANGE:
            SET_BIT(MCUCR, 2);
            CLR_BIT(MCUCR, 3);
            break;
        case EXTINT_SENSE_FALLING_EDGE:
            CLR_BIT(MCUCR, 2);
            SET_BIT(MCUCR, 3);
            break;
        case EXTINT_SENSE_RISING_EDGE:
            SET_BIT(MCUCR, 2);
            SET_BIT(MCUCR, 3);
            break;
        default:
            retVal = E_NOT_OK;
            break;
        }
    }
    else if (EXTINT_SOURCE_INT2 == source)
    {
        switch (sense)
        {
        case EXTINT_SENSE_FALLING_EDGE:
            CLR_BIT(MCUCSR, 6);
            break;
        case EXTINT_SENSE_RISING_EDGE:
            SET_BIT(MCUCSR, 6);
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

StdReturnType ExtInt_EnableInterrupt (ExtInt_SourceType source)
{
    StdReturnType retVal = E_OK;
    if (EXTINT_SOURCE_INT0 == source)
    {
        SET_BIT(GICR, 6);
    }
    else if (EXTINT_SOURCE_INT1 == source)
    {
        SET_BIT(GICR, 7);
    }
    else if (EXTINT_SOURCE_INT2 == source)
    {
        SET_BIT(GICR, 5);
    }
    else 
    {
        retVal = E_NOT_OK;
    }
    return retVal;
}

StdReturnType ExtInt_DisableInterrupt (ExtInt_SourceType source)
{
    StdReturnType retVal = E_OK;
    if (EXTINT_SOURCE_INT0 == source)
    {
        CLR_BIT(GICR, 6);
    }
    else if (EXTINT_SOURCE_INT1 == source)
    {
        CLR_BIT(GICR, 7);
    }
    else if (EXTINT_SOURCE_INT2 == source)
    {
        CLR_BIT(GICR, 5);
    }
    else 
    {
        retVal = E_NOT_OK;
    }
    return retVal;
}


StdReturnType ExtInt_SetCallback (ExtInt_SourceType source, void (*callbackPtr) (void))
{
    StdReturnType retVal = E_OK;
    switch (source)
    {
    case EXTINT_SOURCE_INT0:
        Callbck_Int0 = callbackPtr;
        break;
    case EXTINT_SOURCE_INT1:
        Callbck_Int1 = callbackPtr;
        break;
    case EXTINT_SOURCE_INT2:
        Callbck_Int2 = callbackPtr;
        break;
    default:
        retVal = E_NOT_OK;
        break;
    }
    return retVal;
}