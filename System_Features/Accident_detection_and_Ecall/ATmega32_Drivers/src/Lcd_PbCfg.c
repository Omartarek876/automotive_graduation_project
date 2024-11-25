/*
 * Lcd_PbCfg.c
 *
 *  Created on: Sep 16, 2023
 *      Author: omar tarek
 */
#include "Lcd.h"

Lcd_ControlType Lcd_Configuration = {
    .display = LCD_DISPLAY_ON,
    .lines   = LCD_LINES_2,
    .font    = LCD_FONT_5X7,
    .cusror  = LCD_CURSOR_ON_BLINK_ON
};


