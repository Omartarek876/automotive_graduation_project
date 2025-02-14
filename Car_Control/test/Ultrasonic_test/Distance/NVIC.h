/******************************************************************************
* File Name: <NVIC.h>
* Author : Omar Tarek
* Description: Interface Header file for the NVIC [Nested Vector Interrupt Controller] Driver.
* Date Created: 12/09/2024
* Micro-Controller: TM4C123GH6PM (TIVA C)
* Micro-Processor: ARM Cortex-M4
 ******************************************************************************/


/*Guarding of the File*/
#ifndef _NVIC_INTERFACE_H_
#define _NVIC_INTERFACE_H_
/*****************************/
#include "Platform_Types.h"


/* Enable Exceptions ... This Macro enable IRQ interrupts, Programmble Systems Exceptions and Faults by clearing the I-bit in the PRIMASK. */
#define Enable_Exceptions()    __asm("CPSIE I")

/* Disable Exceptions ... This Macro disable IRQ interrupts, Programmble Systems Exceptions and Faults by clearing the I-bit in the PRIMASK. */
#define Disable_Exceptions()   __asm("CPSID I")

/* Enable Faults ... This Macro enable Faults by clearing the F-bit in the FAULTMASK */
#define Enable_Faults()        __asm("CPSIE F")

/* Disable Faults ... This Macro disable Faults by setting the F-bit in the FAULTMASK */
#define Disable_Faults()       __asm("CPSID F")  

/* Go to low power mode while waiting for the next interrupt */
#define Wait_For_Interrupt()   __asm("WFI")

/* Trigger SVC Exception ... This Macro use the SVC instruction to make SW Interrupt */
#define Trigger_SVC_Exception() __asm("SVC #0") /*TRIGGERED SVC 0*/
/*********************************************************************************************/

/*Vector Table Interrupts Starting from 16 which is Intrrupt Number in Enable and Priorities Registers 
For HW Interrupt not SW*/
#define GPIO_PORTA_INTERRUPT_NUMBER                    ((uint8)0)
#define GPIO_PORTB_INTERRUPT_NUMBER                    ((uint8)1)
#define GPIO_PORTC_INTERRUPT_NUMBER                    ((uint8)2)
#define GPIO_PORTD_INTERRUPT_NUMBER                    ((uint8)3)
#define GPIO_PORTE_INTERRUPT_NUMBER                    ((uint8)4)


#define UART0_INTERRUPT_NUMBER                         ((uint8)5)
#define UART1_INTERRUPT_NUMBER                         ((uint8)6)

#define SSI0_INTERRUPT_NUMBER                          ((uint8)7)

#define I2C0_INTERRUPT_NUMBER                          ((uint8)8)

#define PWM0_FAULT_INTERRUPT_NUMBER                    ((uint8)9)
#define PWM0_GENERATOR0_INTERRUPT_NUMBER               ((uint8)10)
#define PWM0_GENERATOR1_INTERRUPT_NUMBER               ((uint8)11)
#define PWM0_GENERATOR2_INTERRUPT_NUMBER               ((uint8)12)

#define QEI0_INTERRUPT_NUMBER                          ((uint8)13)

#define ADC0_SEQUENCE0_INTERRUPT_NUMBER                ((uint8)14)
#define ADC0_SEQUENCE1_INTERRUPT_NUMBER                ((uint8)15)
#define ADC0_SEQUENCE2_INTERRUPT_NUMBER                ((uint8)16)
#define ADC0_SEQUENCE3_INTERRUPT_NUMBER                ((uint8)17)

#define WATCHDOG_TIMERS_0_1_INTERRUPT_NUMBER           ((uint8)18)

#define TIMER0A_16_32_Bit_INTERRUPT_NUMBER             ((uint8)19)
#define TIMER0B_16_32_Bit_INTERRUPT_NUMBER             ((uint8)20)
#define TIMER1A_16_32_Bit_INTERRUPT_NUMBER             ((uint8)21)
#define TIMER1B_16_32_Bit_INTERRUPT_NUMBER             ((uint8)22)
#define TIMER2A_16_32_Bit_INTERRUPT_NUMBER             ((uint8)23)
#define TIMER2B_16_32_Bit_INTERRUPT_NUMBER             ((uint8)24)

#define ANALOG_COMPARATOR0_INTERRUPT_NUMBER            ((uint8)25)
#define ANALOG_COMPARATOR1_INTERRUPT_NUMBER            ((uint8)26)

#define SYSTEM_CONTROL_INTERRUPT_NUMBER                ((uint8)28)
#define FLASH_MEMORY_CONTROL_and_EEPROM_CONTROL_INTERRUPT_NUMBER               ((uint8)29)

#define GPIO_PORTF_INTERRUPT_NUMBER                    ((uint8)30)
#define UART2_INTERRUPT_NUMBER                         ((uint8)33)

#define SSI1_INTERRUPT_NUMBER                          ((uint8)34)

#define TIMER3A_16_32_Bit_INTERRUPT_NUMBER             ((uint8)35)
#define TIMER3B_16_32_Bit_INTERRUPT_NUMBER             ((uint8)36)

#define I2C1_INTERRUPT_NUMBER                          ((uint8)37)

#define QEI1_INTERRUPT_NUMBER                          ((uint8)38)

#define CAN0_INTERRUPT_NUMBER                          ((uint8)39)
#define CAN1_INTERRUPT_NUMBER                          ((uint8)40)

#define HIBERNATION_MODULE_INTERRUPT_NUMBER            ((uint8)43)
#define USB_INTERRUPT_NUMBER                           ((uint8)44)

#define PWM_GENERATOR3_INTERRUPT_NUMBER                ((uint8)45)

#define MICRO_DMA_SOFTWARE_INTERRUPT_NUMBER            ((uint8)46)
#define MICRO_DMA_ERROR_INTERRUPT_NUMBER               ((uint8)47)

#define ADC1_SEQUENCE0_INTERRUPT_NUMBER                ((uint8)48)
#define ADC1_SEQUENCE1_INTERRUPT_NUMBER                ((uint8)49)
#define ADC1_SEQUENCE2_INTERRUPT_NUMBER                ((uint8)50)
#define ADC1_SEQUENCE3_INTERRUPT_NUMBER                ((uint8)51)

#define SSI2_INTERRUPT_NUMBER                          ((uint8)57)
#define SSI3_INTERRUPT_NUMBER                          ((uint8)58)

#define UART3_INTERRUPT_NUMBER                         ((uint8)59)
#define UART4_INTERRUPT_NUMBER                         ((uint8)60)
#define UART5_INTERRUPT_NUMBER                         ((uint8)61)
#define UART6_INTERRUPT_NUMBER                         ((uint8)62)
#define UART7_INTERRUPT_NUMBER                         ((uint8)63)

#define I2C2_INTERRUPT_NUMBER                          ((uint8)68)
#define I2C3_INTERRUPT_NUMBER                          ((uint8)69)

#define TIMER4A_16_32_Bit_INTERRUPT_NUMBER             ((uint8)70)
#define TIMER4B_16_32_Bit_INTERRUPT_NUMBER             ((uint8)71)
#define TIMER5A_16_32_Bit_INTERRUPT_NUMBER             ((uint8)92)
#define TIMER5B_16_32_Bit_INTERRUPT_NUMBER             ((uint8)93)
#define TIMER0A_32_64_Bit_INTERRUPT_NUMBER             ((uint8)94)
#define TIMER0B_32_64_Bit_INTERRUPT_NUMBER             ((uint8)95)
#define TIMER1A_32_64_Bit_INTERRUPT_NUMBER             ((uint8)96)
#define TIMER1B_32_64_Bit_INTERRUPT_NUMBER             ((uint8)97)
#define TIMER2A_32_64_Bit_INTERRUPT_NUMBER             ((uint8)98)
#define TIMER2B_32_64_Bit_INTERRUPT_NUMBER             ((uint8)99)
#define TIMER3A_32_64_Bit_INTERRUPT_NUMBER             ((uint8)100)
#define TIMER3B_32_64_Bit_INTERRUPT_NUMBER             ((uint8)101)
#define TIMER4A_32_64_Bit_INTERRUPT_NUMBER             ((uint8)102)
#define TIMER4B_32_64_Bit_INTERRUPT_NUMBER             ((uint8)103)
#define TIMER5A_32_64_Bit_INTERRUPT_NUMBER             ((uint8)104)
#define TIMER5B_32_64_Bit_INTERRUPT_NUMBER             ((uint8)105)

#define SYSTEM_EXCEPTION_INTERRUPT_NUMBER              ((uint8)106)

#define PWM1_GENERATOR0_INTERRUPT_NUMBER               ((uint8)134)
#define PWM1_GENERATOR1_INTERRUPT_NUMBER               ((uint8)135)
#define PWM1_GENERATOR2_INTERRUPT_NUMBER               ((uint8)136)
#define PWM1_GENERATOR3_INTERRUPT_NUMBER               ((uint8)137)

#define PWM1_FAULT_INTERRUPT_NUMBER                    ((uint8)138)
/*******************************************************************************/

/*Interrupt Priority Pins Mask*/
#define INTA_PRIORITY_BITS_MASK                              (0xFFFFFF1F)
#define INTB_PRIORITY_BITS_MASK                              (0xFFFF1FFF)
#define INTC_PRIORITY_BITS_MASK                              (0xFF1FFFFF)
#define INTD_PRIORITY_BITS_MASK                              (0x1FFFFFFF)


/*********************************************************************************************/

/*Interrupt Priority Bits Start Positions*/
#define INTA_PRIORITY_START_BIT_POSITION                            ((uint8)5)
#define INTB_PRIORITY_START_BIT_POSITION                            ((uint8)13)
#define INTC_PRIORITY_START_BIT_POSITION                            ((uint8)21)
#define INTD_PRIORITY_START_BIT_POSITION                            ((uint8)29)


/*********************************************************************************************/

/*System Handler Exception Priorities Mask*/
#define MEMORY_FAULT_PRIORITY_MASK                                    (0xFFFFFF1F)
#define BUS_FAULT_PRIORITY_MASK                                       (0xFFFF1FFF)
#define USAGE_FAULT_PRIORITY_MASK                                     (0xFF1FFFFF)
#define SVC_PRIORITY_MASK                                             (0x1FFFFFFF)
#define DEBUG_PRIORITY_MASK                                           (0xFFFFFF1F)
#define PENDSV_PRIORITY_MASK                                          (0xFF1FFFFF)
#define SYSTICK_PRIORITY_MASK                                         (0x1FFFFFFF)


/*********************************************************************************************/

/*System Handler Exception Interrupt Priority Bits Start Positions*/
#define MEMORY_FAULT_PRIORITY_START_BIT_POSITION                      ((uint8)5)
#define BUS_FAULT_PRIORITY_START_BIT_POSITION                         ((uint8)13)
#define USAGE_FAULT_PRIORITY_START_BIT_POSITION                       ((uint8)21)
#define SVC_PRIORITY_START_BIT_POSITION                               ((uint8)29)
#define DEBUG_PRIORITY_START_BIT_POSITION                             ((uint8)5)
#define PENDSV_PRIORITY_START_BIT_POSITION                            ((uint8)21)
#define SYSTICK_PRIORITY_START_BIT_POSITION                           ((uint8)29)


/*********************************************************************************************/

#define PENDSV_ENABLE_BIT                                             ((uint8)28)

/*System Handler Exception Enabling Mask*/
#define MEMORY_MANAGEMENT_FAULT_ENABLE_MASK                           (0xFFFEFFFF)
#define BUS_FAULT_ENABLE_MASK                                         (0xFFFDFFFF)
#define USAGE_FAULT_ENABLE_MASK                                       (0xFFFBFFF7)


#define MEMORY_MANAGEMENT_FAULT_ENABLE_BIT                            ((uint8)16)
#define BUS_FAULT_ENABLE_BIT                                          ((uint8)17)
#define USAGE_FAULT_ENABLE_BIT                                        ((uint8)18)


/********************************************************************************************/

#define MEMORY_MANAGEMENT_INTERRUPT                                   ((uint8)0)
#define BUS_FAULT_INTERRUPT                                           ((uint8)1)
#define USAGE_FAULT_INTERRUPT                                         ((uint8)2)
#define SVC_INTERRUPT                                                 ((uint8)3)
#define DEBUG_MONITOR_INTERRUPT                                       ((uint8)4)
#define PENDSV_INTERRUPT                                              ((uint8)5)
#define SYSTICK_INTERRUPT                                             ((uint8)6)

/********************************************************************************************/

/*Functions Prototypes*/
/*DISABLE MICRO-CONTROLLER INTERRUPT*/
void NVIC_DisableInterrupt(uint8 Interrupt);
/*ENABLE MICRO-CONTROLLER INTERRUPT*/
void NVIC_EnableInterrupt(uint8 Interrupt);
/*SET MICRO-CONTROLLER INTERRUPT PRIORITY*/
void NVIC_SetInterruptPriorityHw(uint8 Interrupt,uint8 priority);
/*SET MICRO-PROCESSOR INTERRUPT PRIORITY*/
void NVIC_SetInterruptPrioritySw(uint8 Interrupt,uint8 priority);
/*CONVERT THE PROCESSOR FROM PRIVILEGED TO UN-PRIVILEGED*/
void Switch_To_Unprivileged(void);
/*CONVERT THE PROCESSOR FROM UN-PRIVILEGED TO PRIVILEGED*/
void Switch_To_Privileged(void);
/*SOFTWARE INTERRUPT INIT*/
void SVC_Init(void);
/*SOFTWARE INTERRUPT HANDLER*/
void SVC_Handler(void);
/*PENDSV INTERRUPT INIT*/
void PendSV_Init(void);
/*PENDSV INTERRUPT CALL*/
void PendSV_Call(void);
/*ENABLE TRIGGERING A FAULT WHEN DIVIDING BY 0*/
void DIV0_faultEnable(void);
/*ENABLE TRIGGERING A FAULT WHEN there's a BUS fault*/
void BUS_FAULT_ENABLE(void);
/*ENABLE TRIGGERING A FAULT WHEN there's a USAGE fault*/
void USAGE_FAULT_ENABLE(void);
/*ENABLE TRIGGERING A FAULT WHEN there's a MEMORY fault*/
void MEMORY_FAULT_ENABLE(void);





#endif
