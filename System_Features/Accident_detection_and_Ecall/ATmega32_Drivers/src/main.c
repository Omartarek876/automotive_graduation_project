/*
 * main.c
 *
 *  Created on: Sep 15, 2023
 *      Author: omar tarek
 */

#include "StdTypes.h"
#include "Registers.h"
#include "Delay.h"
#include "Dio.h"
#include "Lcd.h"
#include "Keypad.h"
#include "Interrupts.h"
#include "Gie.h"
#include "ExtInt.h"
#include "Adc.h"
#include "Lm35.
#include "Gpt.h"
#include "Pwm.h"
#include "Icu.h
#include "Wdt.h"
#include "Uart.h"
#include "Spi.h"
#include "I2c.h"

#include "I2c_Cfg.h"
#if (I2C_CFG_MODE == I2C_MASTER)

void Uart_ReceiveHandler (u8 data)
{
    I2c_ErrorType error;
    Uart_Transmit(data);
    error = I2c_MasterTransmit(0x25, &data, 1);
    if(error != I2C_NO_ERROR)
    {
        Uart_Print("Transmit Error: %d\n", error);
    }
}

int main (void)
{
    Uart_Init();
    I2c_Init();
    Uart_EnableInterrupt(UART_INT_SOURCE_RX);
    Uart_SetReceiveCallback(Uart_ReceiveHandler);
    Gie_Enable();
    while (1)
    {

    }
}


#elif (I2C_CFG_MODE == I2C_SLAVE)

int main (void)
{
    u8 data;
    Uart_Init();
    I2c_Init();
    Gie_Enable();
    while (1)
    {
        I2c_ErrorType error;
        error = I2c_SlaveReceive(&data, 1);
        if(error != I2C_NO_ERROR)
        {
            Uart_Print("Receive Error: %d\n", error);
        }
        else
        {
            Uart_Transmit(data);
        }
    }
}

#endif