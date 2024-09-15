
 /******************************************************************************
* File Name: <UART.C>
* Author : MAHMOUD SAMIR
* Description: SOURCE file for TM4C123GH6PM Microcontroller - UART Driver.
* Date Created: 15/09/2024
* Micro-Controller: TM4C123GH6PM (TIVA C)
* Micro-Processor: ARM Cortex-M4
 ******************************************************************************/





#include "uart.h"
#include "port_Driver/Port.h"
#include "Critical_Files/Common_Macros.h"
#include "Critical_Files/Platform_Types.h"
#include "Critical_Files/Private_registers.h"

// Pointers to access UART and PORT registers
volatile uint32* uart_ptr = NULL_PTR ;
volatile uint32* port_ptr = NULL_PTR ;


  void uartInit(uart_configuration* ptr2config)
{
  uint32 portCLK;

// Select the base address for the specified UART module
  switch(ptr2config->uart_num)
  {
  case 0:
    uart_ptr = (volatile uint32*)UART0_BASE_ADDRESS;
    break;
  case 1:
    uart_ptr = (volatile uint32*)UART1_BASE_ADDRESS;
    break;
  case 2:
    uart_ptr = (volatile uint32*)UART2_BASE_ADDRESS;
    break;
  case 3:
    uart_ptr = (volatile uint32*)UART3_BASE_ADDRESS;
    break;
  case 4:
    uart_ptr = (volatile uint32*)UART4_BASE_ADDRESS;
    break;
  case 5:
    uart_ptr = (volatile uint32*)UART5_BASE_ADDRESS;
    break;
  case 6:
    uart_ptr = (volatile uint32*)UART6_BASE_ADDRESS;
    break;
  case 7:
    uart_ptr = (volatile uint32*)UART7_BASE_ADDRESS;
    break;
 }

// Select the port clock mask for the specified port number
  switch(ptr2config->port_num)
  {
    case 0:
    portCLK =PORTA_SYSCLK_MASK ;
    break;
  case 1:
    portCLK = PORTB_SYSCLK_MASK ;
    break;
  case 2:
    portCLK = PORTC_SYSCLK_MASK;
    break;
  case 3:
    portCLK = PORTD_SYSCLK_MASK;
    break;
  case 4:
    portCLK = PORTE_SYSCLK_MASK;
    break;
  case 5:
    portCLK = PORTF_SYSCLK_MASK;
    break;
  }

// Enable the clock for the specified UART module
  SET_BIT(SYSCTL_RCGCUART_REG,ptr2config->uart_num) ;

// Wait until the UART module is ready
  while((SYSCTL_PRUART_REG& portCLK) == LOGIC_LOW);

// Disable UART before making any configuration changes
  CLEAR_BIT(*(volatile uint32*)((volatile uint8 *)uart_ptr + UART_CTRL_REG_OFFSET),FIRST_BIT);

// Calculate the baud rate integer and fractional parts
  float32 val = (float32)(ptr2config->clk)/(float32)(16*ptr2config->baud_rate) ;
  uint32 int_val = (uint32) val ;
  float32 float_val = (float32)val - (float32)int_val  ;

  uint32 float_reg = (uint32)(float_val*64.0 + 0.5) ; // Calculate the fractional part

// Set the integer part of the baud rate
  *(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_INTEGER_BAUD_RATE_REG_OFFSET) = ((*(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_INTEGER_BAUD_RATE_REG_OFFSET)) & \
    (UART_INTEGER_BAUD_RATE_MASK)) | (int_val << INTDIV_IBRD_REG_START_BIT);


// Set the fractional part of the baud rate
  *(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_FRACTIONAL_BAUD_RATE_REG_OFFSET) = ((*(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_FRACTIONAL_BAUD_RATE_REG_OFFSET)) & \
    (UART_FRACTIONAL_BAUD_RATE_MASK)) | (float_reg << FLOAT_DIV_FBRD_REG_START_BIT);

// Set the clock source for UART
  *(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_CLOCK_SOURCE_REG_OFFSET) = ((*(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_CLOCK_SOURCE_REG_OFFSET)) & \
    (UART_CLOCK_SOURCE_MASK)) | (UART_CLK_FREQUENCY_MASK);


// Enable UART, TX, and RX
  SET_BIT(*(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_CTRL_REG_OFFSET),UART_CTL_ENABLE_BIT);

// Set data length (e.g., 8-bit, 7-bit) in the Line Control Register
  *(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_LINE_CTRL_REG_OFFSET) = ((*(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_LINE_CTRL_REG_OFFSET)) & \
    (UART_LINE_CTRL_SOURCE_MASK)) | (ptr2config->length << LENGTH_LINE_CTRL_REG_START_BIT);


}


void UART_sendByte(const uint8 data)
{

// Wait until the transmit FIFO is not full
   while(((*(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_FLAG_RATE_REG_OFFSET)) & (1 << TRANSMIT_FLAG_BIT)) != 0);

// Write the byte to the data register to send
  *(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_DATA_REG_OFFSET) = data;
}

uint8 uart_RecieveByte(void)
{
  uint8 recieved_byte;

// Wait until the receive FIFO is not empty
  while(((*(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_FLAG_RATE_REG_OFFSET)) & (1 << RECIEVE_FLAG_BIT)) != 0);

// Read the received byte from the data register
  recieved_byte =  *(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_DATA_REG_OFFSET);
  return recieved_byte;
}


void UART_SendString(const uint8 *pData)
{
    uint32 uCounter =0;
    /* Transmit the whole string */
    while(pData[uCounter] != '\0')
    {
        UART_sendByte(pData[uCounter]); /* Send the byte */
        uCounter++; /* increment the counter to the next byte */
    }
}


void UART_ReceiveString(uint8 *pData)
{
    uint32 uCounter =0;

    pData[uCounter] = uart_RecieveByte(); /* Receive the first byte */

    /* Receive the whole string until the '#' */
    while(pData[uCounter] != '#')
    {
        uCounter++;
        pData[uCounter] = uart_RecieveByte();
    }
    /* After receiving the whole string plus the '#', replace the '#' with '\0' */
    pData[uCounter] = '\0';
}


void UART_SendData(const uint8 *pData, uint32 uSize)
{
    /* Transmit the number of bytes requested on the UART port */
    while(uSize--)
    {

        UART_sendByte(*pData); /* Send the byte */
        pData++; /* increment pointer to the next byte */
    }
}


void UART_ReceiveData(uint8 *pData, uint32 uSize)
{
    /* Receive the number of bytes requested on the UART port */
    while(uSize--)
    {
        *pData = uart_RecieveByte(); /* receive a byte from the UART */
         pData++; /* increment pointer to receive the next byte in the next location */
    }
}

