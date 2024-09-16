/*
 * Dio.c
 *
 *  Created on: Sep 15, 2023
 *      Author: omar tarek
 */

#include "StdTypes.h"
#include "Macros.h"
#include "Registers.h"
#include "Dio.h"


StdReturnType Dio_SetPinMode (Dio_PortType port, Dio_PinType pin, Dio_ModeType mode)
{
    StdReturnType retVal = E_OK;
    if (pin > DIO_PIN7)
    {
        retVal = E_NOT_OK;
    }
    else
    {
        switch (port)
        {
        case DIO_PORTA:
            switch (mode)
            {
            case DIO_MODE_OUTPUT:
                SET_BIT(DDRA, pin);
                break;
            case DIO_MODE_INPUT_FLOATING:
                CLR_BIT(DDRA, pin);
                CLR_BIT(PORTA, pin);
                break;
            case DIO_MODE_INPUT_PULLUP:
                CLR_BIT(DDRA, pin);
                SET_BIT(PORTA, pin);
                break;
            default:
                retVal = E_NOT_OK;
                break;
            }
            break;
        case DIO_PORTB:
            switch (mode)
            {
            case DIO_MODE_OUTPUT:
                SET_BIT(DDRB, pin);
                break;
            case DIO_MODE_INPUT_FLOATING:
                CLR_BIT(DDRB, pin);
                CLR_BIT(PORTB, pin);
                break;
            case DIO_MODE_INPUT_PULLUP:
                CLR_BIT(DDRB, pin);
                SET_BIT(PORTB, pin);
                break;
            default:
                retVal = E_NOT_OK;
                break;
            }
            break;
        case DIO_PORTC:
            switch (mode)
            {
            case DIO_MODE_OUTPUT:
                SET_BIT(DDRC, pin);
                break;
            case DIO_MODE_INPUT_FLOATING:
                CLR_BIT(DDRC, pin);
                CLR_BIT(PORTC, pin);
                break;
            case DIO_MODE_INPUT_PULLUP:
                CLR_BIT(DDRC, pin);
                SET_BIT(PORTC, pin);
                break;
            default:
                retVal = E_NOT_OK;
                break;
            }
            break;
        case DIO_PORTD:
            switch (mode)
            {
            case DIO_MODE_OUTPUT:
                SET_BIT(DDRD, pin);
                break;
            case DIO_MODE_INPUT_FLOATING:
                CLR_BIT(DDRD, pin);
                CLR_BIT(PORTD, pin);
                break;
            case DIO_MODE_INPUT_PULLUP:
                CLR_BIT(DDRD, pin);
                SET_BIT(PORTD, pin);
                break;
            default:
                retVal = E_NOT_OK;
                break;
            }
            break;
        default:
            retVal = E_NOT_OK;
            break;
        }
    }
    return retVal;
}

StdReturnType Dio_SetPinLevel(Dio_PortType port, Dio_PinType pin, Dio_LevelType level)
{
    StdReturnType retVal = E_OK;
    if (pin > DIO_PIN7)
    {
        retVal = E_NOT_OK;
    }
    else
    {
        switch (port)
        {
        case DIO_PORTA:
            if (DIO_HIGH == level) 
            {
                SET_BIT(PORTA, pin);
            }
            else if (DIO_LOW == level)
            {
                CLR_BIT(PORTA, pin);
            }
            else
            {
                retVal = E_NOT_OK;
            }
            break;
        case DIO_PORTB:
            if (DIO_HIGH == level) 
            {
                SET_BIT(PORTB, pin);
            }
            else if (DIO_LOW == level)
            {
                CLR_BIT(PORTB, pin);
            }
            else
            {
                retVal = E_NOT_OK;
            }
            break;
        case DIO_PORTC:
            if (DIO_HIGH == level) 
            {
                SET_BIT(PORTC, pin);
            }
            else if (DIO_LOW == level)
            {
                CLR_BIT(PORTC, pin);
            }
            else
            {
                retVal = E_NOT_OK;
            }
            break;
        case DIO_PORTD:
            if (DIO_HIGH == level) 
            {
                SET_BIT(PORTD, pin);
            }
            else if (DIO_LOW == level)
            {
                CLR_BIT(PORTD, pin);
            }
            else
            {
                retVal = E_NOT_OK;
            }
            break;
        default:
            retVal = E_NOT_OK;
            break;
        }
    }
    return retVal;
}

Dio_LevelType Dio_FlipPinLevel(Dio_PortType port, Dio_PinType pin)
{
    Dio_LevelType level = DIO_LOW;
    switch (port)
    {
    case DIO_PORTA:
        TOG_BIT(PORTA, pin); 
        level = GET_BIT(PINA, pin);
        break;
    case DIO_PORTB:
        TOG_BIT(PORTB, pin);
        level = GET_BIT(PINB, pin);
        break;
    case DIO_PORTC:
        TOG_BIT(PORTC, pin);
        level = GET_BIT(PINC, pin);
        break;
    case DIO_PORTD:
        TOG_BIT(PORTD, pin);
        level = GET_BIT(PIND, pin);
        break;
    default:
        break;
    }
    return level;
}

Dio_LevelType Dio_ReadPinLevel(Dio_PortType port, Dio_PinType pin)
{
    Dio_LevelType level = DIO_LOW;
    switch (port)
    {
    case DIO_PORTA:
        level = GET_BIT(PINA, pin);
        break;
    case DIO_PORTB:
        level = GET_BIT(PINB, pin);
        break;
    case DIO_PORTC:
        level = GET_BIT(PINC, pin);
        break;
    case DIO_PORTD:
        level = GET_BIT(PIND, pin);
        break;
    default:
        break;
    }
    return level;
}
