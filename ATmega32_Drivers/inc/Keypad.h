/*
 * Keypad.h
 *
 *  Created on: Sep 22, 2023
 *      Author: omar tarek
 */

#ifndef INC_KEYPAD_H_
#define INC_KEYPAD_H_

typedef enum 
{
    KEYPAD_NOT_PRESSED,
    KEYPAD_PRESSED
} Keypad_StateType;

typedef enum 
{
    KEYPAD_B00,
    KEYPAD_B01,
    KEYPAD_B02,
    KEYPAD_B03,
    KEYPAD_B04,
    KEYPAD_B05,
    KEYPAD_B06,
    KEYPAD_B07,
    KEYPAD_B08,
    KEYPAD_B09,
    KEYPAD_B10,
    KEYPAD_B11,
    KEYPAD_B12,
    KEYPAD_B13,
    KEYPAD_B14,
    KEYPAD_B15,
    KEYPAD_INVALID
} Keypad_ButtonType;

void Keypad_Init(void);
Keypad_StateType Keypad_GetButtonState(Keypad_ButtonType button);
Keypad_ButtonType Keypad_GetPressedButton(void);

#endif /* INC_KEYPAD_H_ */
