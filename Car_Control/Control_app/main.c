/*
 * main.c
 *
 *  Created on:     Aug 13, 2024
 *  Last modified : Dec 20, 2024
 *  Author:         Omar Tarek
 */

#include "Port_Driver/Port.h"
#include "Dio_Driver/Dio.h"
#include "Uart_Driver/uart.h"
#include "Critical_Files/private_registers.h"
#include "control_driver/control_app.h"



int main(void) {
    uint8 dataReceived;
    uint8 Motion_Control_Char;

    car_init();
    uartInit(&uart0_cfg);
    uartInit(&uart2_cfg);

    while (1) {
        dataReceived = uart_RecieveByte(2);
        Motion_Control_Char = dataReceived - 48;
        switch (Motion_Control_Char)
        {
        case 1 :
             car_forword();
             UART_sendByte(0 , dataReceived); // for test
             UART_SendString(2 , "ack1");
             break;
        case 2 :
             car_backword();
             UART_sendByte(0 , dataReceived); // for test
             UART_SendString(2 , "ack2");
             break;
        case 3 :
             car_right ();
             UART_sendByte(0 , dataReceived); // for test
             UART_SendString(2 , "ack3");
             break;
        case 4 :
             car_left ();
             UART_sendByte(0 , dataReceived); // for test
             UART_SendString(2 , "ack4");
             break;
        case 5 :
             car_stop ();
             UART_sendByte(0 , dataReceived); // for test
             UART_SendString(2 , "ack5");
             break;
        default :
            UART_sendByte(0 , dataReceived); // for test
            UART_SendString(0 , "none\n");
            UART_SendString(2 , "ack0");
            break;
        }
    }

}
