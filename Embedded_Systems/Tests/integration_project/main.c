/*
 * main.c
 *
 *  Created on:     Aug 13, 2024
 *  Last modified : Dec 20, 2024
 *  Author:         Omar Tarek
 */


//#include "Uart_Driver/uart.h"
#include "PWM_Driver/PWM.h"
#include "control_driver/control_app.h"



int main(void) {
   // car_init();

    /* Initialize the PWM driver (configures three channels) */
    PWM_Init();

    /* Update the duty cycle for all three PWM channels
     * PB6 , PB4
     * */
    PWM_SetDuty(PWM_CHANNEL_0, 10);
    PWM_SetDuty(PWM_CHANNEL_1, 90);
    // car_forword();
    while(1)
    {

    }
}


/*
#include "Critical_Files/Platform_Types.h"
#include "Port_Driver/Port.h"
#include "Dio_Driver/Dio.h"
#include "PWM_Driver/PWM.h"


// Simple delay function in milliseconds
void delay_ms(uint32 ms)
{
    volatile uint32 i;
    while(ms--)
    {
        for(i = 0; i < 16000; i++) {}  // Roughly 1 ms delay at 16 MHz
    }
}

int main(void)
{
    Port_Init(&Port_Configuration);
    Dio_Init(&Dio_Configuration);

    // Initialize the PWM driver (this configures four channels):
    //   PWM_CHANNEL_0: PB6 (M0PWM0, PWM0 Generator 0 Output A)
    //   PWM_CHANNEL_1: PB4 (M0PWM2, PWM0 Generator 1 Output A)
    //   PWM_CHANNEL_2: PA6 (M1PWM2, PWM1 Generator 1 Output A)
    //   PWM_CHANNEL_3: PA7 (M1PWM3, PWM1 Generator 1 Output B)
    PWM_Init();

    // Reset brightness to off
    PWM_SetDuty(PWM_CHANNEL_0, 10);
    PWM_SetDuty(PWM_CHANNEL_1, 50);
    PWM_SetDuty(PWM_CHANNEL_2, 30);
    PWM_SetDuty(PWM_CHANNEL_3, 90);
    while(1)
    {

    }

}
*/
