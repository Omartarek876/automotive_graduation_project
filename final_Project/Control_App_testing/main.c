
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


uint8  stop_flag = 0;

 int main(void) {
    uint8 dataReceived;
    uint8 Motion_Control_Char = 0;

    car_init();
    PWM_Init();

    PWM_SetDuty(PWM_CHANNEL_1, 40);

    while (1) {

        dataReceived = uart_RecieveByte(1);
        Motion_Control_Char = dataReceived - 48;
        switch (Motion_Control_Char)
            {
        case 0 :
            PWM_SetDuty(PWM_CHANNEL_1, 35);
            break ;
        case 1 :
            PWM_SetDuty(PWM_CHANNEL_1, 40);
            break ;
        case 2 :
            PWM_SetDuty(PWM_CHANNEL_1, 45);
            break ;
        case 3 :
            PWM_SetDuty(PWM_CHANNEL_1, 50);
            break ;

        case 4 :
            if (!stop_flag)
            {
                slow_Stopping();
                stop_flag = 1;
                UART_SendString(1 , "stopping");
            }
            else
            {
                PWM_SetDuty(PWM_CHANNEL_1, 10);
                UART_SendString(1 , "stop again");
            }
            break;

            case 5 :
                PWM_SetDuty(PWM_CHANNEL_1, 10);
                UART_SendString(1 , "stop command");
                stop_flag = 0;
                break;

            case 6 :
                 car_forword();
                 UART_SendString(1 , "forward command");
            //     PWM_SetDuty(PWM_CHANNEL_1, 40);
                 stop_flag = 0;
                 break;

            case 7 :
                 car_backword();
                 UART_SendString(1 , "backword command");
            //     PWM_SetDuty(PWM_CHANNEL_1, 40);
                 stop_flag = 0;
                 break;

            case 8 :
                 car_right ();
                 UART_SendString(1 , "right command");
                 stop_flag = 0;
                 break;
            case 9 :
                 car_left ();
                 UART_SendString(1 , "left command");
                 stop_flag = 0;
                 break;
            }

        }

    }
