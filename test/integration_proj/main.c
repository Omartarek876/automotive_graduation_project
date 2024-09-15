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
    uint8 data;

    Port_Init(&Port_Configuration);
    Dio_Init(&Dio_Configuration);
    uartInit(&uart0_cfg);

    while(1)
    {
        /* Receive byte from PC Terminal */
              data = uart_RecieveByte();

              /* in case the input is from 0 --> 7 provide the corresponding decimal value on LEDs at PF1, PF2 and PF3 */
              if((data >= '0') && (data <= '7'))
              {
                  //GPIO_PORTF_DATA_REG = (GPIO_PORTF_DATA_REG & 0xF1) | ((data - 48)<<1);
                  Dio_WriteChannel(DioConf_LED1_CHANNEL_ID_INDEX,1);  /* LED ON */
              }
    }
}
