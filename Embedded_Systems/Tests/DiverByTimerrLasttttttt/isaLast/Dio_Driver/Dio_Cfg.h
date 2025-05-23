 /******************************************************************************
* File Name: <Dio_Cfg.h>
* Author : Omar Tarek
* Description: Pre-Compile Configuration Header file for TM4C123GH6PM Microcontroller - Dio Driver
* Date Created: 12/09/2024
* Micro-Controller: TM4C123GH6PM (TIVA C)
* Micro-Processor: ARM Cortex-M4
 ******************************************************************************/


#ifndef DIO_CFG_H
#define DIO_CFG_H

/*
 * Module Version 1.0.0
 */
#define DIO_CFG_SW_MAJOR_VERSION              (1U)
#define DIO_CFG_SW_MINOR_VERSION              (0U)
#define DIO_CFG_SW_PATCH_VERSION              (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define DIO_CFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define DIO_CFG_AR_RELEASE_MINOR_VERSION     (0U)
#define DIO_CFG_AR_RELEASE_PATCH_VERSION     (3U)

/* Pre-compile option for Development Error Detect */
#define DIO_DEV_ERROR_DETECT                (STD_ON)

/* Pre-compile option for Version Info API */
#define DIO_VERSION_INFO_API                (STD_ON)

/* Pre-compile option for presence of Dio_FlipChannel API */
#define DIO_FLIP_CHANNEL_API                (STD_ON)

/* Number of the configured Dio Channels */
#define DIO_CONFIGURED_CHANNLES              (4U) /* led , switch */

/* Channel Index in the array of structures in Dio_PBcfg.c */
#define DioConf_ECHO_CHANNEL_ID_INDEX        (uint8)0x00
#define DioConf_TRIGGER_CHANNEL_ID_INDEX     (uint8)0x01
#define DioConf_led1_CHANNEL_ID_INDEX        (uint8)0x02
#define DioConf_led2_CHANNEL_ID_INDEX        (uint8)0x03

/* DIO Configured Port ID's  */
#define DioConf_trigger_port                (Dio_PortType)0 /* PORTA */
#define DioConf_echo_port                   (Dio_PortType)1 /* PORTB */

/* DIO Configured Channel ID's */
#define DioConf_trigger_channel             (Dio_ChannelType)3 /* Pin 3 in PORTA */
#define DioConf_echo_channel                (Dio_ChannelType)2 /* Pin 2 in PORTB */


/* DIO Configured Port ID's  */
#define DioConf_LED1_PORT              (Dio_PortType)5 /* PORTF */
#define DioConf_LED2_PORT                (Dio_PortType)5 /* PORTF */

/* DIO Configured Channel ID's */
#define DioConf_LED1_CHANNEL         (Dio_ChannelType)1 /* Pin 1 in PORTF */
#define DioConf_LED2_CHANNEL         (Dio_ChannelType)3 /* Pin 3 in PORTF */


#endif /* DIO_CFG_H */
