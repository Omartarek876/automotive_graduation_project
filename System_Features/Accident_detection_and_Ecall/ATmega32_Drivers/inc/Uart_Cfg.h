/*
 * Uart_Cfg.h
 *
 *  Created on: Oct 20, 2023
 *      Author: omar tarek
 */

#ifndef INC_UART_CFG_H_
#define INC_UART_CFG_H_

#define UART_PARITY_DISABLED    0
#define UART_PARITY_EVEN        1
#define UART_PARITY_ODD         2

#define UART_OFF                0
#define UART_ON                 1

#define UART_CHAR_5_BIT         1
#define UART_CHAR_6_BIT         2
#define UART_CHAR_7_BIT         3
#define UART_CHAR_8_BIT         4
#define UART_CHAR_9_BIT         5

#define UART_STOP_BITS_1        1
#define UART_STOP_BITS_2        2

/******************** START OF USER CONFIGURATIONS ********************/

/** Options:
 * - Min: F_CPU/65536
 * - Max: F_CPU/8
*/
#define UART_CFG_BAUDRATE   9600UL

/** Options:
 * - UART_ON
 * - UART_OFF
*/
#define UART_CFG_RX_ENABLE  UART_ON
#define UART_CFG_TX_ENABLE  UART_ON

/** Options:
 * - UART_CHAR_5_BIT
 * - UART_CHAR_6_BIT
 * - UART_CHAR_7_BIT
 * - UART_CHAR_8_BIT
 * - UART_CHAR_9_BIT
*/
#define UART_CFG_CHAR_SIZE  UART_CHAR_8_BIT

/** Options:
 * - UART_PARITY_DISABLED
 * - UART_PARITY_EVEN
 * - UART_PARITY_ODD
*/
#define UART_CFG_PARITY     UART_PARITY_EVEN

/** Options:
 * - UART_STOP_BITS_1
 * - UART_STOP_BITS_2
*/
#define UART_CFG_STOP_BITS  UART_STOP_BITS_1

/******************** END OF USER CONFIGURATIONS ********************/

#endif /* INC_UART_CFG_H_ */
