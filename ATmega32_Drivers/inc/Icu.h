/*
 * Icu.h
 *
 *  Created on: Oct 13, 2023
 *      Author: omar tarek
 */

#ifndef INC_ICU_H_
#define INC_ICU_H_

#include "StdTypes.h"
#include "Dio.h"

#define ICU_PIN_ICP1    DIO_PORTD,DIO_PIN6
typedef enum 
{
    ICU_EDGE_FALLING,
    ICU_EDGE_RISING
} Icu_EdgeType;

void Icu_SetTriggerEdge(Icu_EdgeType edge);
void Icu_EnableInterrupt(void);
void Icu_DisableInterrupt(void);
void Icu_SetCallback(void (*callbackPtr)(u16 value));

#endif /* INC_ICU_H_ */
