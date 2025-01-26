/******************************************************************************
* File Name: <Std_Types.h>
* Author : Omar Tarek
* Description: General type definitions
* Date Created: 12/09/2024
* Micro-Controller: TM4C123GH6PM (TIVA C)
* Micro-Processor: ARM Cortex-M4
 ******************************************************************************/


#ifndef STD_TYPES_H
#define STD_TYPES_H

#include "Platform_Types.h"
#include "Compiler.h"

/* Id for the company in the AUTOSAR
 * for example ID = 1000 :) */
#define STD_TYPES_VENDOR_ID                                  (1000U)

/*
 * Module Version 1.0.0
 */
#define STD_TYPES_SW_MAJOR_VERSION                           (1U)
#define STD_TYPES_SW_MINOR_VERSION                           (0U)
#define STD_TYPES_SW_PATCH_VERSION                           (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define STD_TYPES_AR_RELEASE_MAJOR_VERSION                   (4U)
#define STD_TYPES_AR_RELEASE_MINOR_VERSION                   (0U)
#define STD_TYPES_AR_RELEASE_PATCH_VERSION                   (3U)

/*
 *  Describes the standard Return Type Definitions used in the project
 */
typedef uint8  Std_ReturnType;

/*
 * Structure for the Version of the module.
 * This is requested by calling <Module name>_GetVersionInfo()
 */
typedef struct
{
  uint16  vendorID;
  uint16  moduleID;
  uint8 sw_major_version;
  uint8 sw_minor_version;
  uint8 sw_patch_version;
} Std_VersionInfoType;

// ex
typedef enum{
    Disable_EnumType, Enable_EnumType
}Enable_vs_DisableType;

//ex
#define Enable          1
#define Disable         0

#define STD_HIGH        0x01U       /* Standard HIGH */
#define STD_LOW         0x00U       /* Standard LOW */

#define STD_ACTIVE      0x01U       /* Logical state active */
#define STD_IDLE        0x00U       /* Logical state idle */

#define STD_ON          0x01U       /* Standard ON */
#define STD_OFF         0x00U       /* Standard OFF */

#define E_OK            ((Std_ReturnType)0x00U)      /* Function Return OK */
#define E_NOT_OK        ((Std_ReturnType)0x01U)      /* Function Return NOT OK */

#define RETURN_ERROR    0x00U       /* There is Error */
#define RETURN_FINE     0x01U       /* There is No Error */
#define RETURN_NORMAL     0x01U       /* There is No Error */


/*
void Delay_ms(unsigned long long const n){
    volatile unsigned long long  count = 0;
    uint8 i;
    for(i = 0; i < CPU_CLOCK ; i++){
        count = n  * 77 ;
        while(count--);
    }
}
*/


#endif /* STD_TYPES_H */
