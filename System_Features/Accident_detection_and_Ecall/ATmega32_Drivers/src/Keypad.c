/*
 * Keypdad.c
 *
 *  Created on: Sep 22, 2023
 *      Author: omar tarek
 */

#include "StdTypes.h"
#include "Macros.h"
#include "Dio.h"
#include "Delay.h"
#include "Keypad_Cfg.h"
#include "Keypad.h"

static void Keypad_SetRowLevel(u8 row, Dio_LevelType level);

void Keypad_Init(void)
{
    Dio_SetPinMode(KEYPAD_PIN_C0, DIO_MODE_INPUT_PULLUP);
    Dio_SetPinMode(KEYPAD_PIN_C1, DIO_MODE_INPUT_PULLUP);
    Dio_SetPinMode(KEYPAD_PIN_C2, DIO_MODE_INPUT_PULLUP);
    Dio_SetPinMode(KEYPAD_PIN_C3, DIO_MODE_INPUT_PULLUP);
    Dio_SetPinMode(KEYPAD_PIN_R0, DIO_MODE_OUTPUT);
    Dio_SetPinMode(KEYPAD_PIN_R1, DIO_MODE_OUTPUT);
    Dio_SetPinMode(KEYPAD_PIN_R2, DIO_MODE_OUTPUT);
    Dio_SetPinMode(KEYPAD_PIN_R3, DIO_MODE_OUTPUT);
    Dio_SetPinLevel(KEYPAD_PIN_R0, DIO_HIGH);
    Dio_SetPinLevel(KEYPAD_PIN_R1, DIO_HIGH);
    Dio_SetPinLevel(KEYPAD_PIN_R2, DIO_HIGH);
    Dio_SetPinLevel(KEYPAD_PIN_R3, DIO_HIGH);
}

Keypad_StateType Keypad_GetButtonState(Keypad_ButtonType button)
{
    u8 row = button / 4;
    u8 column = button % 4;
    Keypad_StateType buttonState = KEYPAD_NOT_PRESSED;
    
    /* 1. Set Row of Button to Low*/
    Keypad_SetRowLevel(row, DIO_LOW);

    /* 2. Check if Column of Button is Low (Pressed), then double check */
    switch (column)
    {
    case 0:
        if (DIO_LOW == Dio_ReadPinLevel(KEYPAD_PIN_C0))
        {
            _delay_ms(5);
            if (DIO_LOW == Dio_ReadPinLevel(KEYPAD_PIN_C0))
            {
                buttonState = KEYPAD_PRESSED;
            }
        }
        break;
    case 1:
        if (DIO_LOW == Dio_ReadPinLevel(KEYPAD_PIN_C1))
        {
            _delay_ms(5);
            if (DIO_LOW == Dio_ReadPinLevel(KEYPAD_PIN_C1))
            {
                buttonState = KEYPAD_PRESSED;
            }
        }
        break;
    case 2:
        if (DIO_LOW == Dio_ReadPinLevel(KEYPAD_PIN_C2))
        {
            _delay_ms(5);
            if (DIO_LOW == Dio_ReadPinLevel(KEYPAD_PIN_C2))
            {
                buttonState = KEYPAD_PRESSED;
            }
        }
        break;
    case 3:
        if (DIO_LOW == Dio_ReadPinLevel(KEYPAD_PIN_C3))
        {
            _delay_ms(5);
            if (DIO_LOW == Dio_ReadPinLevel(KEYPAD_PIN_C3))
            {
                buttonState = KEYPAD_PRESSED;
            }
        }
        break;
    default:
        break;
    }

    /* 3. Reset Row of Button to High */
    Keypad_SetRowLevel(row, DIO_HIGH);

    return buttonState;
}

static void Keypad_SetRowLevel(u8 row, Dio_LevelType level)
{
    switch (row)
    {
    case 0:
        Dio_SetPinLevel(KEYPAD_PIN_R0, level);
        break;
    case 1:
        Dio_SetPinLevel(KEYPAD_PIN_R1, level);
        break;
    case 2:
        Dio_SetPinLevel(KEYPAD_PIN_R2, level);
        break;
    case 3:
        Dio_SetPinLevel(KEYPAD_PIN_R3, level);
        break;
    default:
        break;
    }

}


Keypad_ButtonType Keypad_GetPressedButton(void)
{
    Keypad_ButtonType button = KEYPAD_INVALID;
    u8 counter;
    for (counter = KEYPAD_B00; counter <= KEYPAD_B15; counter++)
    {
        if(Keypad_GetButtonState(counter) == KEYPAD_PRESSED)
        {
            button = counter;
            break;
        }
    }
    return button;
}