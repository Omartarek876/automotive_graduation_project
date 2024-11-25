#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
//#include "tm4c123gh6pm.h"
#include <string.h>
//#include "GPIO_Driver.h"
#include "../Dio_Driver/Dio.h"
#include "../Critical_Files/Platform_Types.h"
#include "../Critical_Files/Std_Types.h"





#ifndef __Debug_Driver_H__
#define __Debug_Driver_H__

extern void Debug_Led(uint8 color[10],boolean state);										//To Control the led depend on the color
extern void Debug_Intialization(void);													//To Intialize Led Pins

#endif
