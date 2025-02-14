 /******************************************************************************
* File Name: <Dio_PBcfg.c>
* Author : Omar Tarek
* Description: Post Build Configuration Source file for TM4C123GH6PM Microcontroller - Dio Driver
* Date Created: 12/09/2024
* Micro-Controller: TM4C123GH6PM (TIVA C)
* Micro-Processor: ARM Cortex-M4
 ******************************************************************************/


#include "Dio.h"

/*
 * Module Version 1.0.0
 */
#define DIO_PBCFG_SW_MAJOR_VERSION              (1U)
#define DIO_PBCFG_SW_MINOR_VERSION              (0U)
#define DIO_PBCFG_SW_PATCH_VERSION              (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define DIO_PBCFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define DIO_PBCFG_AR_RELEASE_MINOR_VERSION     (0U)
#define DIO_PBCFG_AR_RELEASE_PATCH_VERSION     (3U)

/* AUTOSAR Version checking between Dio_PBcfg.c and Dio.h files */
#if ((DIO_PBCFG_AR_RELEASE_MAJOR_VERSION != DIO_AR_RELEASE_MAJOR_VERSION)\
 ||  (DIO_PBCFG_AR_RELEASE_MINOR_VERSION != DIO_AR_RELEASE_MINOR_VERSION)\
 ||  (DIO_PBCFG_AR_RELEASE_PATCH_VERSION != DIO_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of PBcfg.c does not match the expected version"
#endif

/* Software Version checking between Dio_PBcfg.c and Dio.h files */
#if ((DIO_PBCFG_SW_MAJOR_VERSION != DIO_SW_MAJOR_VERSION)\
 ||  (DIO_PBCFG_SW_MINOR_VERSION != DIO_SW_MINOR_VERSION)\
 ||  (DIO_PBCFG_SW_PATCH_VERSION != DIO_SW_PATCH_VERSION))
  #error "The SW version of PBcfg.c does not match the expected version"
#endif

/* PB structure used with Dio_Init API */
const Dio_ConfigType Dio_Configuration = {
                                             Dioconf_ctrl1_PORT_NUM,Dioconf_ctrl1_CHANNEL_NUM,
                                             Dioconf_ctrl2_PORT_NUM,Dioconf_ctrl2_CHANNEL_NUM,
                                             Dioconf_ctrl3_PORT_NUM,Dioconf_ctrl3_CHANNEL_NUM,
                                             Dioconf_ctrl4_PORT_NUM,Dioconf_ctrl4_CHANNEL_NUM,
                                             Dioconf_ctrl5_PORT_NUM,Dioconf_ctrl5_CHANNEL_NUM,
                                             Dioconf_ctrl6_PORT_NUM,Dioconf_ctrl6_CHANNEL_NUM
				                         };
