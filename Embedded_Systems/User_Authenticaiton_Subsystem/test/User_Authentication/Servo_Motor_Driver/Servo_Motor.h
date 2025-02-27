/*
 * Servo_Motor.h
 *
 *  Created on: Feb 17, 2025
 *      Author: Omar Tarek
 */

#ifndef SERVO_MOTOR_DRIVER_SERVO_MOTOR_H_
#define SERVO_MOTOR_DRIVER_SERVO_MOTOR_H_

#include "Port_Driver/Port.h"
#include "Dio_Driver/Dio.h"
#include "PWM_Driver/PWM.h"
#include "Critical_Files/private_registers.h"

#define NUMBER_OF_ITERATIONS_PER_ONE_MILI_SECOND 364


void Servo_Init(void);
void Servo_Rotation(void);
void Delay_MS(unsigned long long n);


#endif /* SERVO_MOTOR_DRIVER_SERVO_MOTOR_H_ */
