
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

// Global pointer to hold the base address of the UART module
volatile uint32* uart_ptr = NULL_PTR ;


// Function to initialize the UART module based on user configuration
  void uartInit(uart_configuration* ptr2config)
{

// Select the base address of the UART module based on the uart_num provided
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

// Enable the clock for the selected UART module
  SET_BIT(SYSCTL_RCGCUART_REG,ptr2config->uart_num) ;


  // Poll PRUART to check if the specific UART module clock is ready
     switch (ptr2config->uart_num) {
         case 0:
             while ((SYSCTL_PRUART_REG & 0x01) == 0);  // Wait until UART0 is ready
             break;
         case 1:
             while ((SYSCTL_PRUART_REG & 0x02) == 0);  // Wait until UART1 is ready
             break;
         case 2:
             while ((SYSCTL_PRUART_REG & 0x04) == 0);  // Wait until UART2 is ready
             break;
         case 3:
             while ((SYSCTL_PRUART_REG & 0x08) == 0);  // Wait until UART3 is ready
             break;
         case 4:
             while ((SYSCTL_PRUART_REG & 0x10) == 0);  // Wait until UART4 is ready
             break;
         case 5:
             while ((SYSCTL_PRUART_REG & 0x20) == 0);  // Wait until UART5 is ready
             break;
         case 6:
             while ((SYSCTL_PRUART_REG & 0x40) == 0);  // Wait until UART6 is ready
             break;
         case 7:
             while ((SYSCTL_PRUART_REG & 0x80) == 0);  // Wait until UART7 is ready
             break;
         default:
             // Handle invalid UART number if necessary
             break;
     }

    // Disable UART module before configuring it
  CLEAR_BIT(*(volatile uint32*)((volatile uint8 *)uart_ptr + UART_CTRL_REG_OFFSET),FIRST_BIT);

    // Calculate and configure integer and fractional baud rate
  float32 val = (float32)(ptr2config->clk)/(float32)(16*ptr2config->baud_rate) ;
  uint32 int_val = (uint32) val ; // Integer part of baud rate
  float32 float_val = (float32)val - (float32)int_val  ; // Fractional part

   // Convert fractional part to a register value (scaled by 64)
  uint32 float_reg = (uint32)(float_val*64.0 + 0.5) ;

  // Set the integer baud rate
  *(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_INTEGER_BAUD_RATE_REG_OFFSET) = ((*(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_INTEGER_BAUD_RATE_REG_OFFSET)) & \
    (UART_INTEGER_BAUD_RATE_MASK)) | (int_val << INTDIV_IBRD_REG_START_BIT);


  // Set the fractional baud rate
  *(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_FRACTIONAL_BAUD_RATE_REG_OFFSET) = ((*(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_FRACTIONAL_BAUD_RATE_REG_OFFSET)) & \
    (UART_FRACTIONAL_BAUD_RATE_MASK)) | (float_reg << FLOAT_DIV_FBRD_REG_START_BIT);

  // Configure the clock source for the UART
  *(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_CLOCK_SOURCE_REG_OFFSET) = ((*(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_CLOCK_SOURCE_REG_OFFSET)) & \
    (UART_CLOCK_SOURCE_MASK)) | (UART_CLK_FREQUENCY_MASK);

  // Enable UART module after configuration
  SET_BIT(*(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_CTRL_REG_OFFSET),UART_CTL_ENABLE_BIT);

  // Set the data length (e.g., 8-bit, 9-bit) in the Line Control Register
  *(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_LINE_CTRL_REG_OFFSET) = ((*(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_LINE_CTRL_REG_OFFSET)) & \
    (UART_LINE_CTRL_SOURCE_MASK)) | (ptr2config->length << LENGTH_LINE_CTRL_REG_START_BIT);


}

// Function to send a byte via UART
void UART_sendByte(const uint8 data)
{
    // Wait until UART is ready to transmit (check the Transmit Flag)
   while(((*(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_FLAG_RATE_REG_OFFSET)) & (1 << TRANSMIT_FLAG_BIT)) != 0);

    // Write the data to the UART Data Register for transmission
  *(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_DATA_REG_OFFSET) = data;
}

// Function to receive a byte via UART
uint8 uart_RecieveByte(void)
{
  uint8 recieved_byte;

// Wait until UART has received data (check the Receive Flag)
  while(((*(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_FLAG_RATE_REG_OFFSET)) & (1 << RECIEVE_FLAG_BIT)) != 0);

// Read the received byte from the UART Data Register
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

