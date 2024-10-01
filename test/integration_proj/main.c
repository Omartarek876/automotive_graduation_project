/*
 * main.c
 *
 *  Created on: Aug 13, 2024
 *      Author: Omar Tarek
 */

#include <NVIC_Driver/NVIC.h>
#include "Port_Driver/Port.h"
#include "Dio_Driver/Dio.h"
#include "SYSTICK_Driver/SYSTICK.h"
#include "Uart_Driver/uart.h"
#include "Critical_Files/private_registers.h"


/* SysTick Timer ISR ... No need to clear the trigger flag (COUNT) bit ... it cleared automatically by the HW */
/*
void SysTick_Handler(void)
{
    Dio_FlipChannel(DioConf_LED1_CHANNEL_ID_INDEX);
}
*/
/*
void main (void)
{
        Port_Init(&Port_Configuration);

        Dio_Init(&Dio_Configuration);

        SysTick_Init(&systickConfiguration);

        NVIC_EnableInterrupt(30);
    while(1)
    {

    }

}
*/


void main (void)
{

    uint8 receivedString[100]; // Buffer to hold the received String
    uint8 receivedData[10]; // Buffer to hold the received data
    uint32 size = 7; // Number of bytes to receive

    Port_Init(&Port_Configuration);
    Dio_Init(&Dio_Configuration);
    uartInit(&uart0_cfg);


  while(1)
    {
        // Receive a string from UART (until a '#' is received)
        UART_ReceiveString(receivedString);

        // Send the received string back to the terminal
        UART_SendString("Received: ")
        UART_SendString(receivedString);
        UART_SendString("\r\n");

        // Receive a specific number of bytes
        UART_ReceiveData(receivedData, size);

        // Send the received data back to the terminal
        UART_SendString("Received: ");
        UART_SendData(receivedData, size);
        UART_SendString("\r\n");
    }
}
