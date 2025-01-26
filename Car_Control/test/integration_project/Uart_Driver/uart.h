 /******************************************************************************
* File Name: <UART.h>
* Author : MAHMOUD SAMIR
* Description: Header file for TM4C123GH6PM Microcontroller - UART Driver.
* Date Created: 15/9/2024
* Micro-Controller: TM4C123GH6PM (TIVA C)
* Micro-Processor: ARM Cortex-M4
 ******************************************************************************/



#ifndef UART_H
#define UART_H

#include "Critical_Files/Platform_Types.h"


#define UART0_BASE_ADDRESS 0x4000C000
#define UART1_BASE_ADDRESS 0x4000D000
#define UART2_BASE_ADDRESS 0x4000E000
#define UART3_BASE_ADDRESS 0x4000F000
#define UART4_BASE_ADDRESS 0x40010000
#define UART5_BASE_ADDRESS 0x40011000
#define UART6_BASE_ADDRESS 0x40012000
#define UART7_BASE_ADDRESS 0x40013000


#define UART_DATA_REG_OFFSET 0x00
#define UART_RECIEVE_STATUS_REG_OFFSET 0x004
#define UART_FLAG_RATE_REG_OFFSET 0x018
#define UART_LOW_POWER_REG_OFFSET 0x020
#define UART_INTEGER_BAUD_RATE_REG_OFFSET 0x024
#define UART_FRACTIONAL_BAUD_RATE_REG_OFFSET 0x028
#define UART_LINE_CTRL_REG_OFFSET 0x02C
#define UART_CTRL_REG_OFFSET 0x030
#define UART_FIFO_REG_OFFSET 0x034
#define UART_CLOCK_SOURCE_REG_OFFSET 0xFC8



#define UART_CTL_ENABLE_BIT 0
#define UART_INTEGER_BAUD_RATE_MASK 0xFFFF0000
#define UART_FRACTIONAL_BAUD_RATE_MASK 0xFFFFFFE0
#define UART_CLOCK_SOURCE_MASK 0xFFFFFFF0
#define UART_LINE_CTRL_SOURCE_MASK 0xFFFFFF0F
#define UART_CLK_FREQUENCY_MASK 0x00



#define UART_RECIEVE_MODE    (uint8)0x01U
#define UART_TRANSMIT_MODE    (uint8)0x01U


#define LENGTH_LINE_CTRL_REG_START_BIT 5
#define INTDIV_IBRD_REG_START_BIT     0
#define FLOAT_DIV_FBRD_REG_START_BIT     0

#define TRANSMIT_FLAG_BIT 5
#define RECIEVE_FLAG_BIT 4

#define FIRST_BIT 0


/************************************************************************************
* Name: DataSize
* Description: Defining New type as an enum to represent number of data bits
************************************************************************************/


typedef enum
{
  five_bits_data,
  six_bits_data,
  seven_bits_data,
  eight_bits_data
}DataSize;

/************************************************************************************

* Name:uart_configuration
* Parameters: uint32 baud rate
  uint32 CLK
  uint8 uart number
  uint8 port number
  uint8 transmit pin number
  uint8 receive pin number
  DataSize length


* Description: uart configuration .
************************************************************************************/

typedef struct
{
  uint32 baud_rate;
  uint32 clk ;
  uint8 uart_num ;
  DataSize length;
}uart_configuration ;



/************************************************************************************
* Service Name: uartInit
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): Pointer to Structure
* Parameters (out): None
* Return value: None
* Description: Intialize Uart
************************************************************************************/


void uartInit(uart_configuration* ptr2config);



/************************************************************************************
* Service Name: UART_sendByte
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): uint8 Data
* Parameters (out): None
* Return value: None
* Description: API to Send data
************************************************************************************/
void UART_sendByte(const uint8 data);


/************************************************************************************
* Service Name: uart_RecieveByte
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): None
* Parameters (out): uint8 Data
* Return value: uint8
* Description: API to recieve/read data
************************************************************************************/


uint8 uart_RecieveByte(void);


/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_SendString(const uint8 *pData);


/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_ReceiveString(uint8 *pData);


/*
 * Description :
 * Send the required number of bytes through UART to the other UART device.
 */
void UART_SendData(const uint8 *pData, uint32 uSize);


/*
 * Description :
 * receive the required number of bytes through UART from the other UART device.
 */
void UART_ReceiveData(uint8 *pData, uint32 uSize);


/*******************************************************************************
                          External Variables
 *******************************************************************************/

/* Post build structure used with uart_Init API */
extern  uart_configuration uart0_cfg;
extern  uart_configuration uart1_cfg;
extern  uart_configuration uart2_cfg;
extern  uart_configuration uart3_cfg;
extern  uart_configuration uart4_cfg;
extern  uart_configuration uart5_cfg;
extern  uart_configuration uart6_cfg;
extern  uart_configuration uart7_cfg;














#endif
