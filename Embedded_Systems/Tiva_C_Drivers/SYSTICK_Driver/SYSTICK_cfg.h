 /******************************************************************************
* File Name: <SYSTICK_cfg.h>
* Author : Omar Tarek
* Description: Configuration Source file for the SYSTICK TIMER Driver.
* Date Created: 12/09/2024
* Micro-Controller: TM4C123GH6PM (TIVA C)
* Micro-Processor: ARM Cortex-M4
 ******************************************************************************/

#ifndef SYSTICK_DRIVER_SYSTICK_CFG_H_
#define SYSTICK_DRIVER_SYSTICK_CFG_H_



#define SYSTICK_PRIORITY_MASK         (0x1FFFFFFF)
#define SYSTICK_INTERRUPT_PRIORITY    (3)
#define SYSTICK_PRIORITY_BITS_POS     (29)

#define CURRENT_REG                   ((uint8)0)
#define TIME_IN_SEC                   ((uint32)15999999)
#define DIV_FOR_MILLI                 ((uint16)1000)
#define CLEAR_REG                     ((uint8)0x0)
#define MASK_FOR_INTERRUPT_AND_ENABLE ((uint8)0x07)
#define MASK_FOR_SYSTEM_CLOCK         ((uint8)0x4)
#define MASK_FOR_INTERRUPT_ENABLE     ((uint8)0x2)
#define MASK_FOR_ENABLE_SYSTICK       ((uint8)0x01)



#endif /* SYSTICK_DRIVER_SYSTICK_CFG_H_ */
