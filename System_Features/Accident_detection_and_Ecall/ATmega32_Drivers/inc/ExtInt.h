/*
 * ExtInt.h
 *
 *  Created on: Sep 23, 2023
 *      Author: omar tarek
 */

#ifndef INC_EXTINT_H_
#define INC_EXTINT_H_

#include "StdTypes.h"
#include "Dio.h"

#define EXTINT_PIN_INT0 DIO_PORTD,DIO_PIN2
#define EXTINT_PIN_INT1 DIO_PORTD,DIO_PIN3
#define EXTINT_PIN_INT2 DIO_PORTB,DIO_PIN2

typedef enum
{
    EXTINT_SOURCE_INT0,
    EXTINT_SOURCE_INT1,
    EXTINT_SOURCE_INT2,
} ExtInt_SourceType;

typedef enum 
{
    EXTINT_SENSE_LOW_LEVEL,
    EXTINT_SENSE_ON_CHANGE,
    EXTINT_SENSE_FALLING_EDGE,
    EXTINT_SENSE_RISING_EDGE,
} ExtInt_SenseType;

StdReturnType ExtInt_SetSenseControl (ExtInt_SourceType source, ExtInt_SenseType sense);
StdReturnType ExtInt_EnableInterrupt (ExtInt_SourceType source);
StdReturnType ExtInt_DisableInterrupt (ExtInt_SourceType source);
StdReturnType ExtInt_SetCallback (ExtInt_SourceType source, void (*callbackPtr) (void));


#endif /* INC_EXTINT_H_ */
