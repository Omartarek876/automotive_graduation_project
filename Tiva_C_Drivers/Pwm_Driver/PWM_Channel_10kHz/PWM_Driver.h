#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
//#include "tm4c123gh6pm.h"
#include <string.h>
#include "Debug_Driver.h"
#include "../Critical_Files/Private_registers.h"
#include "../Critical_Files/Platform_Types.h"


#ifndef __PWM_Driver_H__
#define __PWM_Driver_H__


extern void CLK_PWM_Enable(uint32 PWM_module);                                                              //Clock for the module
extern void Use_PWM_Divisor(uint32 System_Clock,uint32 PWM_Clock);                                             //To set PWM Clock
extern void PWM_Generator_Enable(uint32 PWM_module,uint32 Generator,char type[10]);                            //Enable or Disable specific Generator
extern void PWM_Action_For_Load(uint32 PWM_module,uint32 Generator,char Comparator,char type[10]);             //Action to be taken when the counter matches the value in the load register
extern void PWM_Action_Comparator_Down(uint32 PWM_module,uint32 Generator,char Comparator,char type[10]);      //Action to be taken when the counter matches the value in the Comparator
extern void PWM_Set_Frequency(uint32 PWM_module,uint32 Generator,uint32 Frequency,uint32 PWM_Clock_MHz);             //Set the Frequency for the pwm
extern void PWM_Set_Duty_Cycle(uint32 PWM_module,uint32 Generator,char Comparator,uint32 Duty);                   //To determine the duty cycle
extern void PWM_Module_Start(uint32 PWM_module);                                                            //To start the pwm Module


#endif
