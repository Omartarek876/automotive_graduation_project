/*
 * main.c
 *
 *  Created on: Aug 13, 2024
 *  last modified : nov 25, 2024
 *      Author: Omar Tarek
 */


#include "control_driver/control_app.h"
#include "Uart_Driver/uart.h"

void main (void)
{
    car_init();
    uartInit(&uart0_cfg);

    //uint8 receivedString[100]; // Buffer to hold the received String
    //uint8 receivedData[10]; // Buffer to hold the received data
    //uint32 size = 7; // Number of bytes to receive

    while(1)
    {
        // Receive a char from UART to control the motion of the car
        uint8 Motion_Control_Char = uart_RecieveByte();
        UART_sendByte(Motion_Control_Char);
        UART_SendString(" \n");
        switch (Motion_Control_Char)
        {
        case 'f' :
            UART_SendString("car Forward\r\n");
             car_forword();
             break;
        case 'b' :
            UART_SendString("car Backward\r\n");
             car_backword();
             break;
        case 'r' :
            UART_SendString("car right\r\n");
             car_right ();
             break;
        case 'l' :
            UART_SendString("car left\r\n");
             car_left ();
             break;
        case 's' :
            UART_SendString("car stop\r\n");
             car_stop ();
             break;
        default :
            UART_SendString("not an option\r\n");
            break;
        }
/*
        // Send the received string back to the terminal
        UART_SendString("Received: ");
        UART_SendString(receivedString);
        UART_SendString("\r\n");
*/
/*
        // Receive a specific number of bytes
        UART_ReceiveData(receivedData, size);

        // Send the received data back to the terminal
        UART_SendString("Received: ");
        UART_SendData(receivedData, size);
        UART_SendString("\r\n");
*/

      /*
        Delay_MS(5000);
        Dio_WriteChannel(DIoconf_ctrl1_CHANNEL_ID_INDEX , STD_HIGH);
        Dio_WriteChannel(DIoconf_ctrl2_CHANNEL_ID_INDEX , STD_LOW);
        Dio_WriteChannel(DIoconf_ctrl3_CHANNEL_ID_INDEX , STD_HIGH);
        Dio_WriteChannel(DIoconf_ctrl4_CHANNEL_ID_INDEX , STD_LOW);
        Delay_MS(3000);
        Dio_WriteChannel(DIoconf_ctrl1_CHANNEL_ID_INDEX , STD_LOW);
        Dio_WriteChannel(DIoconf_ctrl2_CHANNEL_ID_INDEX , STD_LOW);
        Dio_WriteChannel(DIoconf_ctrl3_CHANNEL_ID_INDEX , STD_LOW);
        Dio_WriteChannel(DIoconf_ctrl4_CHANNEL_ID_INDEX , STD_LOW);
        Delay_MS(500);
        Dio_WriteChannel(DIoconf_ctrl5_CHANNEL_ID_INDEX , STD_HIGH);
        Dio_WriteChannel(DIoconf_ctrl6_CHANNEL_ID_INDEX , STD_LOW);
        Delay_MS(10000);
        Dio_WriteChannel(DIoconf_ctrl6_CHANNEL_ID_INDEX , STD_HIGH);
        Dio_WriteChannel(DIoconf_ctrl5_CHANNEL_ID_INDEX , STD_LOW);
        Delay_MS(10000);
        Dio_WriteChannel(DIoconf_ctrl6_CHANNEL_ID_INDEX , STD_LOW);
        Dio_WriteChannel(DIoconf_ctrl5_CHANNEL_ID_INDEX , STD_LOW);
*/


    }

}
