/*
 * Keypad_Cfg.h
 *
 *  Created on: Sep 22, 2023
 *      Author: omar tarek
 */

#ifndef INC_KEYPAD_CFG_H_
#define INC_KEYPAD_CFG_H_

#include "Dio.h"


/***********************************************
*   B00   *   B01   *   B02   *   B03   *  ROW0*
*   B04   *   B05   *   B06   *   B07   *  ROW1*
*   B08   *   B09   *   B10   *   B11   *  ROW2*
*   B12   *   B13   *   B14   *   B15   *  ROW3*
* COLOMN0   COLUMN1   COLUMN2   COLUMN3 *      *
***********************************************/

#define KEYPAD_PIN_R0   DIO_PORTD,DIO_PIN0
#define KEYPAD_PIN_R1   DIO_PORTD,DIO_PIN1
#define KEYPAD_PIN_R2   DIO_PORTD,DIO_PIN2
#define KEYPAD_PIN_R3   DIO_PORTD,DIO_PIN3
#define KEYPAD_PIN_C0   DIO_PORTD,DIO_PIN4
#define KEYPAD_PIN_C1   DIO_PORTD,DIO_PIN5
#define KEYPAD_PIN_C2   DIO_PORTD,DIO_PIN6
#define KEYPAD_PIN_C3   DIO_PORTD,DIO_PIN7

#endif /* INC_KEYPAD_CFG_H_ */
