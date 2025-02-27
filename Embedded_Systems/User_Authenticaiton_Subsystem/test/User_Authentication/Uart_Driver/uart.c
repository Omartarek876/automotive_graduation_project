
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


volatile uint32* uart_ptr = NULL_PTR ;
volatile uint32* uart_bases[8] = {
    (volatile uint32*)UART0_BASE_ADDRESS,
    (volatile uint32*)UART1_BASE_ADDRESS,
    (volatile uint32*)UART2_BASE_ADDRESS,
    (volatile uint32*)UART3_BASE_ADDRESS,
    (volatile uint32*)UART4_BASE_ADDRESS,
    (volatile uint32*)UART5_BASE_ADDRESS,
    (volatile uint32*)UART6_BASE_ADDRESS,
    (volatile uint32*)UART7_BASE_ADDRESS
};

  void uartInit(uart_configuration* ptr2config)
{
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

  CLEAR_BIT(*(volatile uint32*)((volatile uint8 *)uart_ptr + UART_CTRL_REG_OFFSET),FIRST_BIT);

  float32 val = (float32)(ptr2config->clk)/(float32)(16*ptr2config->baud_rate) ; 
  uint32 int_val = (uint32) val ; 
  float32 float_val = (float32)val - (float32)int_val  ; 
  
  uint32 float_reg = (uint32)(float_val*64.0 + 0.5) ; 
  *(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_INTEGER_BAUD_RATE_REG_OFFSET) = ((*(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_INTEGER_BAUD_RATE_REG_OFFSET)) & \
    (UART_INTEGER_BAUD_RATE_MASK)) | (int_val << INTDIV_IBRD_REG_START_BIT);
  
  
  
  *(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_FRACTIONAL_BAUD_RATE_REG_OFFSET) = ((*(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_FRACTIONAL_BAUD_RATE_REG_OFFSET)) & \
    (UART_FRACTIONAL_BAUD_RATE_MASK)) | (float_reg << FLOAT_DIV_FBRD_REG_START_BIT);
  
  *(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_CLOCK_SOURCE_REG_OFFSET) = ((*(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_CLOCK_SOURCE_REG_OFFSET)) & \
    (UART_CLOCK_SOURCE_MASK)) | (UART_CLK_FREQUENCY_MASK);
  
  SET_BIT(*(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_CTRL_REG_OFFSET),UART_CTL_ENABLE_BIT);

  
  *(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_LINE_CTRL_REG_OFFSET) = ((*(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_LINE_CTRL_REG_OFFSET)) & \
    (UART_LINE_CTRL_SOURCE_MASK)) | (ptr2config->length << LENGTH_LINE_CTRL_REG_START_BIT);
  

}

/*
void UART_sendByte(const uint8 data)
{
   while(((*(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_FLAG_RATE_REG_OFFSET)) & (1 << TRANSMIT_FLAG_BIT)) != 0);
  
  *(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_DATA_REG_OFFSET) = data;
}
*/
  void UART_sendByte(uint8 uart_num, const uint8 data)
  {
      volatile uint32* u_ptr = uart_bases[uart_num];
      while(((*(volatile uint32 *)((volatile uint8 *)u_ptr + UART_FLAG_RATE_REG_OFFSET)) & (1 << TRANSMIT_FLAG_BIT)) != 0);
      *(volatile uint32 *)((volatile uint8 *)u_ptr + UART_DATA_REG_OFFSET) = data;
  }
/*
uint8 uart_RecieveByte(void)
{
  uint8 recieved_byte;
  while(((*(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_FLAG_RATE_REG_OFFSET)) & (1 << RECIEVE_FLAG_BIT)) != 0);
  recieved_byte =  *(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_DATA_REG_OFFSET);
  return recieved_byte;
}
*/
  uint8 uart_RecieveByte(uint8 uart_num)
  {
      volatile uint32* u_ptr = uart_bases[uart_num];
      uint8 recieved_byte;
      while(((*(volatile uint32 *)((volatile uint8 *)u_ptr + UART_FLAG_RATE_REG_OFFSET)) & (1 << RECIEVE_FLAG_BIT)) != 0);
      recieved_byte =  *(volatile uint32 *)((volatile uint8 *)u_ptr + UART_DATA_REG_OFFSET);
      return recieved_byte;
  }
/*
void UART_SendString(const uint8 *pData)
{
    uint32 uCounter =0;
    while(pData[uCounter] != '\0')
    {
        UART_sendByte(pData[uCounter]);
        uCounter++;
    }
}
*/
void UART_SendString(uint8 uart_num, const uint8 *pData)
{
    uint32 uCounter = 0;

    /* Transmit the whole string */
    while (pData[uCounter] != '\0')
    {
        UART_sendByte(uart_num, pData[uCounter]);
        uCounter++;
    }
}
/*
void UART_ReceiveString(uint8 *pData)
{
    uint32 uCounter =0;

    pData[uCounter] = uart_RecieveByte();

    while(pData[uCounter] != '#')
    {
        uCounter++;
        pData[uCounter] = uart_RecieveByte();
    }
    pData[uCounter] = '\0';
}
*/
void UART_ReceiveString(uint8 uart_num, uint8 *pData)
{
    uint32 uCounter = 0;

    pData[uCounter] = uart_RecieveByte(uart_num);

    while (pData[uCounter] != '#')
    {
        uCounter++;
        pData[uCounter] = uart_RecieveByte(uart_num);
    }

    pData[uCounter] = '\0';
}
/*
void UART_SendData(const uint8 *pData, uint32 uSize)
{
    while(uSize--)
    {

        UART_sendByte(*pData) ;
        pData++;
    }
}
*/
void UART_SendData(uint8 uart_num, const uint8 *pData, uint32 uSize)
{
    /* Transmit the number of bytes requested on the UART port */
    while (uSize--)
    {
        UART_sendByte(uart_num, *pData);
        pData++;
    }
}
/*
void UART_ReceiveData(uint8 *pData, uint32 uSize)
{
    // Receive the number of bytes requested on the UART port
    while(uSize--)
    {
        *pData = uart_RecieveByte(); // receive a byte from the UART
         pData++; //increment pointer to receive the next byte in the next location
    }
}
*/
void UART_ReceiveData(uint8 uart_num, uint8 *pData, uint32 uSize)
{
    /* Receive the number of bytes requested on the UART port */
    while (uSize--)
    {
        *pData = uart_RecieveByte(uart_num);
        pData++;
    }
}

