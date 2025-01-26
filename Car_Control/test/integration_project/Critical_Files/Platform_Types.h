/******************************************************************************
* File Name: <Platform_Types.h>
* Author : Omar Tarek
* Description: Platform types for ARM Cortex-M4F.
* Date Created: 12/09/2024
* Micro-Controller: TM4C123GH6PM (TIVA C)
* Micro-Processor: ARM Cortex-M4
 ******************************************************************************/


#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H

/* Id for the company in the AUTOSAR
 * for example ID = 1000 :) */
#define PLATFORM_VENDOR_ID                          (1000U)

/*
 * Module Version 1.0.0
 */
#define PLATFORM_SW_MAJOR_VERSION                   (1U)
#define PLATFORM_SW_MINOR_VERSION                   (0U)
#define PLATFORM_SW_PATCH_VERSION                   (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PLATFORM_AR_RELEASE_MAJOR_VERSION           (4U)
#define PLATFORM_AR_RELEASE_MINOR_VERSION           (0U)
#define PLATFORM_AR_RELEASE_PATCH_VERSION           (3U)

/*
 * CPU register type width
 */
#define CPU_TYPE_8                                  (8U)
#define CPU_TYPE_16                                 (16U)
#define CPU_TYPE_32                                 (32U)

/*
 * Bit order definition
 */
#define MSB_FIRST                   (0u)        /* Big endian bit ordering        */
#define LSB_FIRST                   (1u)        /* Little endian bit ordering     */

/*
 * Byte order definition
 */
#define HIGH_BYTE_FIRST             (0u)        /* Big endian byte ordering       */
#define LOW_BYTE_FIRST              (1u)        /* Little endian byte ordering    */

/*
 * Platform type and endianess definitions, specific for ARM Cortex-M4F
 */
#define CPU_TYPE            CPU_TYPE_32
#define CPU_CLOCK           (uint16)20U
#define CPU_BIT_ORDER       LSB_FIRST
#define CPU_BYTE_ORDER      LOW_BYTE_FIRST

/*
 * Boolean Values
 */
#ifndef FALSE
#define FALSE       (0u)
#endif
#ifndef TRUE
#define TRUE        (1u)
#endif
#ifndef ERROR
#define ERROR   (-1)
#endif

#ifndef LOGIC_LOW
#define LOGIC_LOW   (0u)
#endif

#ifndef LOGIC_HIGH
#define LOGIC_HIGH    (1u)
#endif


//NULL POINTER
#define NULL_PTR  ((void*)0)



void Delay_ms(unsigned long long const n);

//ex
#define REG_ORING_WITH_VALUE(REG, VAL) (REG |= VAL)
#define REG_ORING_VALUE_NO_CASTING(REG, VAL) (REG |= VAL)
#define REG_ORING_ONE_BIT_NO_CASTING(REG, CNT) (REG |= 1<<CNT)
#define REG_CLEAR_ONE_BIT_NO_CASTING(REG, CNT) (REG &= (~ (1<<CNT)))

#define REG_ORING_POINTED_REG(REG, VAL)     ((*REG) |= VAL)

#define REG_CLEAR_CASTING_POINTED(REG)       ( (*((volatile uint32 *)(REG))) &= 0x00000000)
#define REG_CLEAR_THOSE_BITS_CASTING_POINTED(REG, VAL)  ( (*((volatile uint32 *)(REG))) &= ~(VAL))
#define REG_ORING_CASTING_POINTED(REG, VAL)  ( (*((volatile uint32 *)(REG))) |= VAL)
#define REG_WRITE_CASTING_POINTED(REG, VAL)  ( (*((volatile uint32 *)(REG))) = VAL)
#define REG_ORING_ONE_BIT_CASTING_POINTED(REG, CNT)     ( (*((volatile uint32 *)(REG))) |= (1<<(CNT) ))
#define REG_CLEAR_ONE_BIT_CASTING_POINTED(REG, CNT)     ( (*((volatile uint32 *)(REG))) &= ~(1<<(CNT)))

#define REG_READ_CASTING_POINTED(x, REG)              (x = ( (*((volatile uint32 *)(REG))) ) )


#define REG_SET_PEIPTH_BB_PTR(REG,BIT_NUM)      ( (volatile uint32)((uint8*)0x42000000 + ( (((uint8*)(REG)-(uint8*)0x40000000) * 32) + ((BIT_NUM) * 4))) = 1)
#define REG_CLR_PERIPH_BB_PTR(REG,BIT_NUM)      ( (volatile uint32)((uint8*)0x42000000 + ( (((uint8*)(REG)-(uint8*)0x40000000) * 32) + ((BIT_NUM) * 4))) = 0)




typedef unsigned char         boolean;

typedef unsigned char         uint8;          /*           0 .. 255             */
typedef signed char           sint8;          /*        -128 .. +127            */
typedef unsigned short        uint16;         /*           0 .. 65535           */
typedef signed short          sint16;         /*      -32768 .. +32767          */
typedef unsigned long         uint32;         /*           0 .. 4294967295      */
typedef signed long           sint32;         /* -2147483648 .. +2147483647     */
typedef unsigned long long    uint64;         /*       0..18446744073709551615  */
typedef signed long long      sint64;         /* -9223372036854775808 .. 9223372036854775807 */
typedef float                 float32;
typedef double                float64;

//char* int_to_string(uint32 var);

//void selectionSort(sint32 arr[], uint8 n);

void WriteUsingBB(uint32* Ptr, uint32 Value);


#endif /* PLATFORM_TYPES_H */
