/*
 * main.c
 *
 *  Created on:     Aug 13, 2024
 *  Last modified : Dec 20, 2024
 *  Author:         Omar Tarek
 */


//#include "Uart_Driver/uart.h"
#include "control_driver/control_app.h"

uint8 dataReceived;
uint8 Motion_Control_Char;


int main(void) {
    car_init();
    car_forword();
    while(1)
    {
        dataReceived = uart_RecieveByte(2);
        Motion_Control_Char = dataReceived - 48;
        switch (Motion_Control_Char)
        {
        case 1 :
             car_forword();
             UART_SendString(2 , "ACK1");
             break;
        case 2 :
             car_backword();
             UART_SendString(2 , "ACK2");
             break;
        case 3 :
             car_right ();
             UART_SendString(2 , "ACK3");
             break;
        case 4 :
             car_left ();
             UART_SendString(2 , "ACK4");
             break;
        case 5 :
             car_stop ();
             UART_SendString(2 , "ACK5");
             break;
        default :
            UART_SendString(2 , "N-ACK");
            break;
        }
    }
}

