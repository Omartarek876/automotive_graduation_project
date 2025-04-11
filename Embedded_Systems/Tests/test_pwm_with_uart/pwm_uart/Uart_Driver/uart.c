
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
    // Select the base address based on UART number
    switch (ptr2config->uart_num)
    {
        case 0: uart_ptr = (volatile uint32*)UART0_BASE_ADDRESS; break;
        case 1: uart_ptr = (volatile uint32*)UART1_BASE_ADDRESS; break;
        case 2: uart_ptr = (volatile uint32*)UART2_BASE_ADDRESS; break;
        case 3: uart_ptr = (volatile uint32*)UART3_BASE_ADDRESS; break;
        case 4: uart_ptr = (volatile uint32*)UART4_BASE_ADDRESS; break;
        case 5: uart_ptr = (volatile uint32*)UART5_BASE_ADDRESS; break;
        case 6: uart_ptr = (volatile uint32*)UART6_BASE_ADDRESS; break;
        case 7: uart_ptr = (volatile uint32*)UART7_BASE_ADDRESS; break;
        default: return; // Invalid UART number
    }

    // Enable UART clock
    SET_BIT(SYSCTL_RCGCUART_REG, ptr2config->uart_num);

    // Wait until UART clock is ready
    while ((SYSCTL_PRUART_REG & (1 << ptr2config->uart_num)) == 0);

    // Disable UART before configuration
    CLEAR_BIT(*(volatile uint32*)((volatile uint8 *)uart_ptr + UART_CTRL_REG_OFFSET), FIRST_BIT);

    // Calculate integer & fractional baud rate divisors
    float32 val = (float32)(ptr2config->clk) / (float32)(16 * ptr2config->baud_rate);
    uint32 int_val = (uint32)val;
    float32 float_val = val - (float32)int_val;
    uint32 float_reg = (uint32)(float_val * 64.0 + 0.5);

    // Set IBRD (integer part)
    *(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_INTEGER_BAUD_RATE_REG_OFFSET) =
        ((*(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_INTEGER_BAUD_RATE_REG_OFFSET)) &
         UART_INTEGER_BAUD_RATE_MASK) | (int_val << INTDIV_IBRD_REG_START_BIT);

    // Set FBRD (fractional part)
    *(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_FRACTIONAL_BAUD_RATE_REG_OFFSET) =
        ((*(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_FRACTIONAL_BAUD_RATE_REG_OFFSET)) &
         UART_FRACTIONAL_BAUD_RATE_MASK) | (float_reg << FLOAT_DIV_FBRD_REG_START_BIT);

    // 🔥 FIX: Force UART clock source to system clock (0x0)
    *(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_CLOCK_SOURCE_REG_OFFSET) = 0x0;

    // Enable UART
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_CTRL_REG_OFFSET), UART_CTL_ENABLE_BIT);

    // Configure data length (e.g., 8-bit)
    *(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_LINE_CTRL_REG_OFFSET) =
        ((*(volatile uint32 *)((volatile uint8 *)uart_ptr + UART_LINE_CTRL_REG_OFFSET)) &
         UART_LINE_CTRL_SOURCE_MASK) | (ptr2config->length << LENGTH_LINE_CTRL_REG_START_BIT);
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

