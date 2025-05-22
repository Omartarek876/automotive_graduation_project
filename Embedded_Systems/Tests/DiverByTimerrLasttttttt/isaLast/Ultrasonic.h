
/*
 *
 *
 * Created on: 21/2/2025
 *      Author: Mariam
 */

#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#include "Critical_Files/Std_Types.h"
#include "Critical_Files/Common_Macros.h"
#include "Critical_Files/private_registers.h"
#include "Dio_Driver/Dio.h"
#include "Port_Driver/Port.h"
#include"Timer0.h"


void ultrasonic_1 ( void );
//uint32 ultrasonic_2 ( void );
uint32 Measure_distance(void);

#endif    /*  ULTRASONIC_H_  */
