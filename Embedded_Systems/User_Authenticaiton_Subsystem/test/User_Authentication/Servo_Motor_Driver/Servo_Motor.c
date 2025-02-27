/*
 * Servo_Motor.c
 *
 *  Created on: Feb 17, 2025
 *      Author: Omar Tarek
 */

#include "Servo_Motor.h"


uint32 dutyCycle ;

void Servo_Init(void){
    Port_Init(&Port_Configuration);
    Dio_Init(&Dio_Configuration);
    PWM_Init();

}

void Servo_Rotation(void) {
        PWM_SetDuty(PWM_CHANNEL_2 , 1);  // Set PWM duty cycle
        Delay_MS(50);  // Delay for smooth movement
        PWM_SetDuty(PWM_CHANNEL_2 , 0);  // Set PWM duty cycle
  //  }
}

void Delay_MS(unsigned long long n)
{
    volatile unsigned long long count = 0;
    while(count++ < (NUMBER_OF_ITERATIONS_PER_ONE_MILI_SECOND * n) );
}

