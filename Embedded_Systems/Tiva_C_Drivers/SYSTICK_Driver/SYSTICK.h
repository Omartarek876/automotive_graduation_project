 /******************************************************************************
* File Name: <SYSTICK.h>
* Author : Omar Tarek
* Description: Interface Header file for the SYSTICK TIMER Driver.
* Date Created: 12/09/2024
* Micro-Controller: TM4C123GH6PM (TIVA C)
* Micro-Processor: ARM Cortex-M4
 ******************************************************************************/

/*Guarding of the File*/
#ifndef _SYSTICK_INTERFACE_H_
#define _SYSTICK_INTERFACE_H_
/*****************************/


#include "../Critical_Files/Platform_Types.h"
#include "SYSTICK_cfg.h"


typedef enum
 {
   systemClkSrcDiv4,
   systemClkSrc
 }CLOCK;   

typedef enum
 {
   disableInt,
   enableInt
 }INTEN;   
 
typedef enum
 {
   LEVEL0,
   LEVEL1,
   LEVEL2,
   LEVEL3,
   LEVEL4,
   LEVEL5,
   LEVEL6,
   LEVEL7
 }INT_PRIORITY;   
  
 

typedef struct {
    uint32 period;          //how many sec all in all
    uint32 DIV;             // indicate unit
    CLOCK clockSource;
    INTEN enableIRQ;
    INT_PRIORITY irqPriority;
} SysTick_ConfigType;



/* External Variables */
/* Extern PB structures to be used by Dio and other modules */
extern const SysTick_ConfigType systickConfiguration;

#define STOP_FUNCTION_AVAILABLE






/*************************** Function Prototypes*************************
 Service Name: Systick_Init
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): Tick_Time - Time in miliseconds
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Setup the SysTick Timer configuration to count in miliseconds:
*              - Set the Reload value
*              - Enable SysTick Timer with System clock 16Mhz
*              - Enable SysTick Timer Interrupt and set its priority
************************************************************************************/	

void Systick_Init(SysTick_ConfigType *ptr2configg);


/************************************************************************************
* Service Name: SysTick_Stop
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Stop the SysTick Timer.
************************************************************************************/

void SysTick_Stop(void);

/************************************************************************************
* Service Name: SysTick_SetCallBack
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): FuncPointer - Call Back function address
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Setup the SysTick Timer call back
************************************************************************************/
void SysTick_SetCallBack(void (*FuncPointer)(void));



#endif
