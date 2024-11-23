#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
//#include "tm4c123gh6pm.h"
#include <string.h>
#include "../Critical_Files/Private_registers.h"
#include "../Critical_Files/Platform_Types.h"





#ifndef __Clock_Driver_H__
#define __Clock_Driver_H__


#define CLK_5Mhz        (0x09)                                                                            //Value for 5MHz
#define CLK_6Mhz        (0x0B)                                                                            //Value for 6MHz
#define CLK_8Mhz        (0x0E)                                                                            //Value for 8MHz
#define CLK_10Mhz       (0x10)                                                                            //Value for 10MHz
#define CLK_12Mhz       (0x11)                                                                            //Value for 12MHz
#define CLK_16Mhz       (0x15)                                                                            //Value for 16MHz
#define CLK_18Mhz       (0x17)                                                                            //Value for 18MHz
#define CLK_20Mhz       (0x18)                                                                            //Value for 20MHz
#define CLK_24Mhz       (0x19)                                                                            //Value for 24MHz
#define CLK_25Mhz       (0x1A)                                                                            //Value for 25MHz


extern void Set_Clock_MHz(uint32 Crystal_Frequency_Mhz, uint32 System_Clock);                                   //Set clk for system

#endif
