/*
 * control_app.h
 *
 *  Created on: Nov 21, 2024
 *      Author: Omar Tarek
 */

#ifndef CONTROL_DRIVER_CONTROL_APP_H_
#define CONTROL_DRIVER_CONTROL_APP_H_

#include "Port_Driver/Port.h"
#include "Dio_Driver/Dio.h"
#include "PWM_Driver/PWM.h"
#include "Uart_Driver/uart.h"
#include "Critical_Files/private_registers.h"


#define NUMBER_OF_ITERATIONS_PER_ONE_MILI_SECOND 364

void Delay_MS(unsigned long long n);
void car_init (void);
void car_forword (void);
void car_backword (void);
void car_right (void);
void car_left (void);
void car_stop (void);
void slow_Stopping (void);

#endif /* CONTROL_DRIVER_CONTROL_APP_H_ */
