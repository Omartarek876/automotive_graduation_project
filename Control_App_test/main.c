
/*
 * main.c
 *
 *  Created on:     Aug 13, 2024
 *  Last modified : Dec 20, 2024
 *  Author:         Omar Tarek
 */


#include "Uart_Driver/uart.h"
#include "control_driver/control_app.h"
#include "PWM_Driver/PWM.h"

 int main(void) {
    uint8 dataReceived;
    uint8 Motion_Control_Char = 0;
    uint8 pre_Command = 0;
  //uint8 i = 0;

    car_init();
    PWM_Init();
    car_stop();
    PWM_SetDuty(PWM_CHANNEL_1, 30);

    while (1) {

    /*    car_forword();
        PWM_SetDuty(PWM_CHANNEL_1, 40);
        Delay_MS(500);  // Wait 100ms between steps
        PWM_SetDuty(PWM_CHANNEL_1, 35);
        Delay_MS(500);  // Wait 100ms between steps
        car_forword();
        PWM_SetDuty(PWM_CHANNEL_1, 30);
        Delay_MS(500);  // Wait 100ms between steps
        PWM_SetDuty(PWM_CHANNEL_1, 25);
        Delay_MS(500);  // Wait 100ms between steps
        car_forword();
        PWM_SetDuty(PWM_CHANNEL_1, 20);
        Delay_MS(500);  // Wait 100ms between steps
        PWM_SetDuty(PWM_CHANNEL_1, 15);
        Delay_MS(500);  // Wait 100ms between steps
        PWM_SetDuty(PWM_CHANNEL_1, 10);
        Delay_MS(500);  // Wait 100ms between steps
        */

        dataReceived = uart_RecieveByte(1);
        Motion_Control_Char = dataReceived - 48;
        pre_Command = Motion_Control_Char;

        if (pre_Command != Motion_Control_Char)
        {
            switch (Motion_Control_Char)
                 {
                 case 1 :
                      car_forword();
                      PWM_SetDuty(PWM_CHANNEL_1, 30);
                      UART_SendString(1 , "ack1");
                      break;
                 case 2 :
                      car_backword();
                 //     PWM_SetDuty(PWM_CHANNEL_1, 60);
                      UART_SendString(1 , "ACK2");
                      break;
                 case 3 :
                      car_right ();
                     UART_SendString(1 , "ACK3");
                      break;
                 case 4 :
                      car_left ();
                     UART_SendString(1 , "ACK4");
                      break;
                 case 5 :
                 case 0 :
                    PWM_SetDuty(PWM_CHANNEL_1, 10);
                    car_stop ();
                    UART_SendString(1 , "ACK5");
                      break;
                 default :
                    UART_SendString(1 , "SPEED");
                    PWM_SetDuty(PWM_CHANNEL_1, Motion_Control_Char);
                     break;
                 }

        }

    }
}
