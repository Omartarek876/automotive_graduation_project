 /******************************************************************************
*File Name: <NVIC_cfg.h>
* Author : Omar Tarek
* Description: Configuration Header file for the NVIC [Nested Vector Interrupt Controller] Driver.
* Date Created: 28/04/2024
* Micro-Controller: TM4C123GH6PM (TIVA C)
* Micro-Processor: ARM Cortex-M4
 ******************************************************************************/


/*Guarding of the File*/
#ifndef _NVIC_CONFIG_H_
#define _NVIC_CONFIG_H_
/*****************************/
/*USER CHOICES*/
#define SVC_INTERRUPT_PRIORITY                        ((uint8)0)
#define PENDSV_INTERRUPT_PRIORITY                     ((uint8)7)       /* Lowest Priority Level*/
#define DEBUG_MONITOR_INTERRUPT_PRIORITY              ((uint8)7)       /* Lowest Priority Level*/
#define USAGE_FAULT_INTERRUPT_PRIORITY                ((uint8)7)       /* Lowest Priority Level*/
#define BUS_FAULT_INTERRUPT_PRIORITY                  ((uint8)7)       /* Lowest Priority Level*/
#define MEMORY_FAULT_INTERRUPT_PRIORITY               ((uint8)7)       /* Lowest Priority Level*/



#endif
