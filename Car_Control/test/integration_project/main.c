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



int main(void) {
    uint8 byteReceived;
    uint8 data[5];
    uint8 receivedstr[20];
    Port_Init(&Port_Configuration);

    uartInit(&uart0_cfg);
    uartInit(&uart2_cfg);

    while (1) {
        // Receive and Send Byte by Byte
/*
        byteReceived = uart_RecieveByte(1);  // Receive from UART1
        UART_sendByte(0, byteReceived);      // Send to UART0
*/
/*
        UART_ReceiveString(2, receivedstr); // Receive string from UART2
        UART_SendString(0, receivedstr);   // Send string to UART0
        UART_SendString(0, "\r\n");
*/

        UART_ReceiveData(2,data,5);
        UART_SendData(0, data, 5);

    }

}
