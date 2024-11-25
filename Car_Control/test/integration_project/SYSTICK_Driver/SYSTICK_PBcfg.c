 /******************************************************************************
* File Name: <SYSTICK_PBcfg.h>
* Author : Omar Tarek
* Description: Configuration Source file for the SYSTICK TIMER Driver.
* Date Created: 12/09/2024
* Micro-Controller: TM4C123GH6PM (TIVA C)
* Micro-Processor: ARM Cortex-M4
 ******************************************************************************/


#include "SYSTICK.h"

const SysTick_ConfigType systickConfiguration =
{
     .period = 1000,         //how many sec all in all
     .DIV =1000,             // indicate unit
     .clockSource = systemClkSrc,
     .enableIRQ = enableInt, //0 for disable and 1 for enable
     .irqPriority = LEVEL0
};
