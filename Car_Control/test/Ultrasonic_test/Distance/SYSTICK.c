 /******************************************************************************
* File Name: <SYSTICK.c>
* Author : Omar Tarek
* Description: Program Source file for the SYSTICK TIMER Driver.
* Date Created: 12/09/2024
* Micro-Controller: TM4C123GH6PM (TIVA C)
* Micro-Processor: ARM Cortex-M4
 ******************************************************************************/


#include <NVIC.h>
#include "private_registers.h"
#include "SYSTICK.h"
#include "Common_Macros.h"

/* Global pointer to function used to point upper layer functions
 * to be used in Call Back */

static void (*SysTick_Call_Back_Pointer)(void) = NULL_PTR;

/************************************************************************************
* Service Name: SysTick_Handler
* Description: SysTick Timer ISR
************************************************************************************/
/*
void SysTick_Handler(void)
{
   // Check if the Timer0_setCallBack is already called
    if(SysTick_Call_Back_Pointer != NULL_PTR)
   {
       (*SysTick_Call_Back_Pointer)(); // call the function in the scheduler using call-back concept
   }
    // No need to clear the trigger flag (COUNT) bit ... it cleared automatically by the HW
}
*/

/************************************************************************************
* Service Name: SysTick_Start
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): Tick_Time - Time in miliseconds
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Setup the SysTick Timer configuration to count n miliseconds:
*              - Set the Reload value
*              - Enable SysTick Timer with System clock 16Mhz
*              - Enable SysTick Timer Interrupt and set its priority
************************************************************************************/
void SysTick_Init(SysTick_ConfigType *ptr2configg)
{
 
    SYSTICK_CTRL_REG    = CLEAR_REG;                                                                   /* Disable the SysTick Timer by Clear the ENABLE Bit */
    SYSTICK_RELOAD_REG  = ((TIME_IN_SEC / (ptr2configg->DIV)) *(ptr2configg->period));                 /* Set the Reload value to count n miliseconds */
    SYSTICK_CURRENT_REG = CURRENT_REG;                                                                 /* Clear the Current Register value */
 
    /* Configure the SysTick Control Register 
     * Enable the SysTick Timer (ENABLE = 1)
     * Enable SysTick Interrupt (INTEN = 1)
     * Choose the clock source to be System Clock (CLK_SRC = 1) */
    
    if(ptr2configg->clockSource == systemClkSrc)
    {
      SYSTICK_CTRL_REG |= MASK_FOR_SYSTEM_CLOCK;
    }
    else if(ptr2configg->clockSource == systemClkSrcDiv4)
    {
        CLEAR_BIT(SYSTICK_CTRL_REG,2);
    }

    if(ptr2configg->enableIRQ == enableInt)
    {
        SYSTICK_CTRL_REG |= MASK_FOR_INTERRUPT_ENABLE;
        /* Assign priority level 3 to the SysTick Interrupt */
       //NVIC_SYSTEM_PRI3_REG =  (NVIC_SYSTEM_PRI3_REG & SYSTICK_PRIORITY_MASK) | ((ptr2configg->irqPriority) << SYSTICK_PRIORITY_BITS_POS);
       NVIC_SetInterruptPrioritySw(SYSTICK_INTERRUPT,ptr2configg->irqPriority); 
    }
    else if(ptr2configg->enableIRQ == disableInt)
    {
        CLEAR_BIT(SYSTICK_CTRL_REG,1);
    }
    SYSTICK_CTRL_REG |= MASK_FOR_ENABLE_SYSTICK;
}

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
#ifdef STOP_FUNCTION_AVAILABLE
void SysTick_Stop(void)
{
    SYSTICK_CTRL_REG = CLEAR_REG; /* Disable the SysTick Timer by Clear the ENABLE Bit */
}
#endif
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
void SysTick_SetCallBack(void(*FuncPointer)(void))
{
    SysTick_Call_Back_Pointer = FuncPointer;
}


