 /******************************************************************************
* File Name: <UART_PBcfg.C>
* Author : MAHMOUD SAMIR
* Description: Post Build file for TM4C123GH6PM Microcontroller - UART Driver.
* Date Created: 15/9/2024
* Micro-Controller: TM4C123GH6PM (TIVA C)
* Micro-Processor: ARM Cortex-M4
 ******************************************************************************/


#include "uart.h"


uart_configuration uart0_cfg ={9600,16000000,0,0,eight_bits_data};
uart_configuration uart1_cfg ={9600,16000000,1,2,eight_bits_data};
uart_configuration uart2_cfg ={9600,16000000,2,3,eight_bits_data};
uart_configuration uart3_cfg ={9600,16000000,3,2,eight_bits_data};
uart_configuration uart4_cfg ={9600,16000000,4,2,eight_bits_data};
uart_configuration uart5_cfg ={9600,16000000,5,4,eight_bits_data};
uart_configuration uart6_cfg ={9600,16000000,6,3,eight_bits_data};
uart_configuration uart7_cfg ={9600,16000000,7,4,eight_bits_data};


