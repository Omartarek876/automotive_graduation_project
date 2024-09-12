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
#include "Critical_Files/private_registers.h"


/* SysTick Timer ISR ... No need to clear the trigger flag (COUNT) bit ... it cleared automatically by the HW */
void SysTick_Handler(void)
{
    Dio_FlipChannel(DioConf_LED1_CHANNEL_ID_INDEX);
}


void main (void)
{
        /* Initialize Port Driver */
        Port_Init(&Port_Configuration);

        /* Initialize Dio Driver */
        Dio_Init(&Dio_Configuration);

        SysTick_Init(&systickConfiguration);

        NVIC_EnableInterrupt(30);
    while(1)
    {

    }

}
