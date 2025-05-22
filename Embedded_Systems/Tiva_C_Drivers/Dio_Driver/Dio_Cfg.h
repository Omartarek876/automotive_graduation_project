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
#define DIO_CONFIGURED_CHANNLES              (7U)


#define DIoconf_ctrl1_CHANNEL_ID_INDEX       (uint8)0x00
#define DIoconf_ctrl2_CHANNEL_ID_INDEX       (uint8)0x01
#define DIoconf_ctrl3_CHANNEL_ID_INDEX       (uint8)0x02
#define DIoconf_ctrl4_CHANNEL_ID_INDEX       (uint8)0x03

#define DIoconf_ctrl5_CHANNEL_ID_INDEX       (uint8)0x04
#define DIoconf_ctrl6_CHANNEL_ID_INDEX       (uint8)0x05


#define Dioconf_ctrl1_PORT_NUM              (Dio_PortType)3 /* PORTD */
#define Dioconf_ctrl2_PORT_NUM              (Dio_PortType)3 /* PORTD */
#define Dioconf_ctrl3_PORT_NUM              (Dio_PortType)3 /* PORTD */
#define Dioconf_ctrl4_PORT_NUM              (Dio_PortType)3 /* PORTD */

#define Dioconf_ctrl5_PORT_NUM              (Dio_PortType)3 /* PORTD */
#define Dioconf_ctrl6_PORT_NUM              (Dio_PortType)3 /* PORTD */

#define Dioconf_ctrl1_CHANNEL_NUM            (Dio_ChannelType)0 /* Pin 0 in PORTD */
#define Dioconf_ctrl2_CHANNEL_NUM            (Dio_ChannelType)1 /* Pin 1 in PORTD */
#define Dioconf_ctrl3_CHANNEL_NUM            (Dio_ChannelType)2 /* Pin 2 in PORTD */
#define Dioconf_ctrl4_CHANNEL_NUM            (Dio_ChannelType)3 /* Pin 3 in PORTD */

#define Dioconf_ctrl5_CHANNEL_NUM            (Dio_ChannelType)6 /* Pin 6 in PORTD */
#define Dioconf_ctrl6_CHANNEL_NUM            (Dio_ChannelType)7 /* Pin 7 in PORTD */

#define ledtest (uint8)0x06
#define ledchannel (Dio_ChannelType)4
#define ledport (Dio_PortType)0

#endif /* DIO_CFG_H */
