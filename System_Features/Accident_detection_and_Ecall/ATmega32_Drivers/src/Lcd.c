/*
 * Lcd.c
 *
 *  Created on: Sep 16, 2023
 *      Author: omar tarek
 */

#include <stdarg.h>
#include "StdTypes.h"
#include "Macros.h"
#include "Dio.h"
#include "Delay.h"
#include "Lcd_Cfg.h"
#include "Lcd.h"

static void Lcd_SendData (u8 data);
static void Lcd_SendCommand (u8 command);

void Lcd_Init(const Lcd_ControlType* config)
{
    /* Set LCD Pins to Output */
    Dio_SetPinMode(LCD_PIN_RS, DIO_MODE_OUTPUT);
    Dio_SetPinMode(LCD_PIN_RW, DIO_MODE_OUTPUT);
    Dio_SetPinMode(LCD_PIN_EN, DIO_MODE_OUTPUT);
#if (LCD_MODE == LCD_MODE_8_BIT)
    Dio_SetPinMode(LCD_PIN_D0, DIO_MODE_OUTPUT);
    Dio_SetPinMode(LCD_PIN_D1, DIO_MODE_OUTPUT);
    Dio_SetPinMode(LCD_PIN_D2, DIO_MODE_OUTPUT);
    Dio_SetPinMode(LCD_PIN_D3, DIO_MODE_OUTPUT);
#endif
    Dio_SetPinMode(LCD_PIN_D4, DIO_MODE_OUTPUT);
    Dio_SetPinMode(LCD_PIN_D5, DIO_MODE_OUTPUT);
    Dio_SetPinMode(LCD_PIN_D6, DIO_MODE_OUTPUT);
    Dio_SetPinMode(LCD_PIN_D7, DIO_MODE_OUTPUT);

#if (LCD_MODE == LCD_MODE_4_BIT)
    /* Enter 4-Bit Mode */
    //Lcd_SendCommand(0x02);
    Dio_SetPinLevel(LCD_PIN_RS, DIO_LOW);
    Dio_SetPinLevel(LCD_PIN_RW, DIO_LOW);
    Dio_SetPinLevel(LCD_PIN_D4, DIO_LOW);
    Dio_SetPinLevel(LCD_PIN_D5, DIO_HIGH);
    Dio_SetPinLevel(LCD_PIN_D6, DIO_LOW);
    Dio_SetPinLevel(LCD_PIN_D7, DIO_LOW);
    Dio_SetPinLevel(LCD_PIN_EN, DIO_HIGH);
    _delay_ms(2);
    Dio_SetPinLevel(LCD_PIN_EN, DIO_LOW);
#endif

    Lcd_ControlDisplay(config);

    /* Display Clear */
    Lcd_SendCommand(0b00000001);
}

void Lcd_DisplayCharacter (u8 data)
{
    Lcd_SendData(data);
}

void Lcd_DisplayString (char str[])
{
    while (*str != 0)
    {
        Lcd_SendData(*str);
        str++;
    }
}

void Lcd_DisplayNumber (s32 number)
{
    u32 reversed = 0;
    u8 digitsCounter = 0;
    if (number < 0)
    {
        Lcd_SendData('-');
        number *= -1;
    }

    do
    {
        digitsCounter++;
        reversed = (reversed*10) + (number%10);
        number /= 10;
    } while (number != 0);

    while (digitsCounter > 0)
    {
        Lcd_SendData(reversed%10 + '0');
        reversed /= 10;
        digitsCounter--;
    }
}

void Lcd_ClearDisplay (void)
{
    Lcd_SendCommand(0b00000001);
}

void Lcd_ControlDisplay(const Lcd_ControlType* control)
{

    u8 command;
    /* Function Set (0 0 1 1 N F X X) */
    command = 0b00110000;
    if (control->lines == LCD_LINES_2)
    {
        SET_BIT(command, 3);
    }
    if (control->font == LCD_FONT_5X10)
    {
        SET_BIT(command, 2);
    }
    Lcd_SendCommand(command);

    /* Display ON/OFF Control (0 0 0 0 1 D C B) */
    command = 0b00001000;
    if (control->display == LCD_DISPLAY_ON)
    {
        SET_BIT(command, 2);
    }
    if (control->cusror == LCD_CURSOR_ON_BLINK_OFF)
    {
        SET_BIT(command, 1);
    }
    else if (control->cusror == LCD_CURSOR_ON_BLINK_ON)
    {
        SET_BIT(command, 1);
        SET_BIT(command, 0);
    }
    Lcd_SendCommand(command);
}

void Lcd_ReturnHome (void)
{
    Lcd_SendCommand(0b00000010);
}

void Lcd_ShiftDisplayRight (u8 count)
{
    while (count > 0)
    {
        Lcd_SendCommand(0b00011100);
        count--;
    }
}

void Lcd_ShiftDisplayLeft (u8 count)
{
        while (count > 0)
    {
        Lcd_SendCommand(0b00011000);
        count--;
    }
}

void Lcd_SetCursorPosition (u8 row, u8 column)
{
    u8 address = column + 0x40*row;
    SET_BIT(address, 7);
    Lcd_SendCommand(address);
}

void Lcd_Print (char* str, ...)
{
    va_list var_list;
    va_start(var_list, str);
    while(*str != 0)
    {
        if (*str == '%')
        {
            f64 number;
            u64 temp;
            str++;
            switch (*str)
            {
            case 'd':
                Lcd_DisplayNumber(va_arg(var_list, int));
                break;
            case 'c':
                Lcd_DisplayCharacter((char)va_arg(var_list, int));
                break;
            case 'l':
                if (*(str+1) == 'd')
                {
                    Lcd_DisplayNumber(va_arg(var_list, long int));
                    str++;
                }
                break;
            case 'u':
                if (*(str+1) == 'l')
                {
                    Lcd_DisplayNumber(va_arg(var_list, unsigned long int));
                    str++;
                }
                else
                {
                    Lcd_DisplayNumber(va_arg(var_list, unsigned int));
                }
                break;
            case 'f':
                number = va_arg(var_list, double);
                temp = (int)number;
                Lcd_DisplayNumber(temp);
                Lcd_DisplayCharacter('.');
                number -= temp;
                Lcd_DisplayNumber((int)(number*100));
                break;
            default:
                break;
            }
        }
        else
        {
            Lcd_DisplayCharacter(*str);
        }
        str++;
    }
}


static void Lcd_SendData (u8 data)
{
    Dio_SetPinLevel(LCD_PIN_RS, DIO_HIGH);
    Dio_SetPinLevel(LCD_PIN_RW, DIO_LOW);
#if (LCD_MODE == LCD_MODE_8_BIT)
    Dio_SetPinLevel(LCD_PIN_D0, GET_BIT(data, 0));
    Dio_SetPinLevel(LCD_PIN_D1, GET_BIT(data, 1));
    Dio_SetPinLevel(LCD_PIN_D2, GET_BIT(data, 2));
    Dio_SetPinLevel(LCD_PIN_D3, GET_BIT(data, 3));
    Dio_SetPinLevel(LCD_PIN_D4, GET_BIT(data, 4));
    Dio_SetPinLevel(LCD_PIN_D5, GET_BIT(data, 5));
    Dio_SetPinLevel(LCD_PIN_D6, GET_BIT(data, 6));
    Dio_SetPinLevel(LCD_PIN_D7, GET_BIT(data, 7));
    Dio_SetPinLevel(LCD_PIN_EN, DIO_HIGH);
    _delay_ms(2);
    Dio_SetPinLevel(LCD_PIN_EN, DIO_LOW);  
#elif (LCD_MODE == LCD_MODE_4_BIT)
    Dio_SetPinLevel(LCD_PIN_D4, GET_BIT(data, 4));
    Dio_SetPinLevel(LCD_PIN_D5, GET_BIT(data, 5));
    Dio_SetPinLevel(LCD_PIN_D6, GET_BIT(data, 6));
    Dio_SetPinLevel(LCD_PIN_D7, GET_BIT(data, 7));
    Dio_SetPinLevel(LCD_PIN_EN, DIO_HIGH);
    _delay_ms(2);
    Dio_SetPinLevel(LCD_PIN_EN, DIO_LOW);
    Dio_SetPinLevel(LCD_PIN_D4, GET_BIT(data, 0));
    Dio_SetPinLevel(LCD_PIN_D5, GET_BIT(data, 1));
    Dio_SetPinLevel(LCD_PIN_D6, GET_BIT(data, 2));
    Dio_SetPinLevel(LCD_PIN_D7, GET_BIT(data, 3));
    Dio_SetPinLevel(LCD_PIN_EN, DIO_HIGH);
    _delay_ms(2);
    Dio_SetPinLevel(LCD_PIN_EN, DIO_LOW);
#else
    #error "[LCD]: Invalid LCD Mode, Options: LCD_MODE_4_BIT, LCD_MODE_8_BIT"
#endif
}

static void Lcd_SendCommand (u8 command)
{
    Dio_SetPinLevel(LCD_PIN_RS, DIO_LOW);
    Dio_SetPinLevel(LCD_PIN_RW, DIO_LOW);
#if (LCD_MODE == LCD_MODE_8_BIT)
    Dio_SetPinLevel(LCD_PIN_D0, GET_BIT(command, 0));
    Dio_SetPinLevel(LCD_PIN_D1, GET_BIT(command, 1));
    Dio_SetPinLevel(LCD_PIN_D2, GET_BIT(command, 2));
    Dio_SetPinLevel(LCD_PIN_D3, GET_BIT(command, 3));
    Dio_SetPinLevel(LCD_PIN_D4, GET_BIT(command, 4));
    Dio_SetPinLevel(LCD_PIN_D5, GET_BIT(command, 5));
    Dio_SetPinLevel(LCD_PIN_D6, GET_BIT(command, 6));
    Dio_SetPinLevel(LCD_PIN_D7, GET_BIT(command, 7));
    Dio_SetPinLevel(LCD_PIN_EN, DIO_HIGH);
    _delay_ms(2);
    Dio_SetPinLevel(LCD_PIN_EN, DIO_LOW);
#elif (LCD_MODE == LCD_MODE_4_BIT)
    Dio_SetPinLevel(LCD_PIN_D4, GET_BIT(command, 4));
    Dio_SetPinLevel(LCD_PIN_D5, GET_BIT(command, 5));
    Dio_SetPinLevel(LCD_PIN_D6, GET_BIT(command, 6));
    Dio_SetPinLevel(LCD_PIN_D7, GET_BIT(command, 7));
    Dio_SetPinLevel(LCD_PIN_EN, DIO_HIGH);
    _delay_ms(2);
    Dio_SetPinLevel(LCD_PIN_EN, DIO_LOW);
    Dio_SetPinLevel(LCD_PIN_D4, GET_BIT(command, 0));
    Dio_SetPinLevel(LCD_PIN_D5, GET_BIT(command, 1));
    Dio_SetPinLevel(LCD_PIN_D6, GET_BIT(command, 2));
    Dio_SetPinLevel(LCD_PIN_D7, GET_BIT(command, 3));
    Dio_SetPinLevel(LCD_PIN_EN, DIO_HIGH);
    _delay_ms(2);
    Dio_SetPinLevel(LCD_PIN_EN, DIO_LOW);
#else
    #error "[LCD]: Invalid LCD Mode, Options: LCD_MODE_4_BIT, LCD_MODE_8_BIT"
#endif
}
