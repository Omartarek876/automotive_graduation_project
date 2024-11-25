/*
 * Lcd.h
 *
 *  Created on: Sep 16, 2023
 *      Author: omar tarek
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

#include "StdTypes.h"

typedef enum
{
    LCD_LINES_1,
    LCD_LINES_2
} Lcd_LinesType;

typedef enum 
{
    LCD_FONT_5X7,
    LCD_FONT_5X10
} Lcd_FontType;

typedef enum 
{
    LCD_DISPLAY_OFF,
    LCD_DISPLAY_ON
} Lcd_DisplayType;

typedef enum 
{
    LCD_CURSOR_OFF,
    LCD_CURSOR_ON_BLINK_ON,
    LCD_CURSOR_ON_BLINK_OFF
} Lcd_CursorType;


typedef struct 
{
    Lcd_DisplayType display;
    Lcd_LinesType lines;
    Lcd_FontType font;
    Lcd_CursorType cusror;
} Lcd_ControlType;

extern Lcd_ControlType Lcd_Configuration;

void Lcd_Init(const Lcd_ControlType* config);
void Lcd_ControlDisplay(const Lcd_ControlType* control);
void Lcd_DisplayCharacter (u8 data);
void Lcd_DisplayString (char str[]);
void Lcd_DisplayNumber (s32 number);
void Lcd_ClearDisplay (void);
void Lcd_ReturnHome (void);
void Lcd_ShiftDisplayRight (u8 count);
void Lcd_ShiftDisplayLeft (u8 count);
void Lcd_SetCursorPosition (u8 row, u8 column);
void Lcd_Print (char* str, ...);

#endif /* INC_LCD_H_ */
