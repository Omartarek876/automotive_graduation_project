/*
 * Spi_Cfg.h
 *
 *  Created on: Oct 21, 2023
 *      Author: omar tarek
 */

#ifndef INC_SPI_CFG_H_
#define INC_SPI_CFG_H_

#define SPI_MASTER                  1
#define SPI_SLAVE                   2

#define SPI_LSB_FIRST               1
#define SPI_MSB_FIRST               2


#define SPI_SCK_IDLE_LOW            1
#define SPI_SCK_IDLE_HIGH           2

#define SPI_SAMPLE_THEN_SETUP       1
#define SPI_SETUP_THEN_SAMPLE       2

#define SPI_PRESCALER_2             1
#define SPI_PRESCALER_4             2
#define SPI_PRESCALER_8             3
#define SPI_PRESCALER_16            4
#define SPI_PRESCALER_32            5
#define SPI_PRESCALER_64            6
#define SPI_PRESCALER_128           7

/******************** START OF USER CONFIGURATIONS ********************/

/** Options:
 * - SPI_MASTER
 * - SPI_SLAVE
*/
#define SPI_CFG_MODE                SPI_MASTER

/** Options:
 * - SPI_LSB_FIRST
 * - SPI_MSB_FIRST
*/
#define SPI_CFG_DATA_ORDER          SPI_LSB_FIRST

/** Options:
 * - SPI_SCK_IDLE_LOW
 * - SPI_SCK_IDLE_HIGH
*/
#define SPI_CFG_CLOCK_POLARITY      SPI_SCK_IDLE_LOW

/** Options:
 * - SPI_SAMPLE_THEN_SETUP
 * - SPI_SETUP_THEN_SAMPLE
*/
#define SPI_CFG_CLOCK_PHASE         SPI_SAMPLE_THEN_SETUP

/** Options:
 * - SPI_PRESCALER_2  
 * - SPI_PRESCALER_4  
 * - SPI_PRESCALER_8  
 * - SPI_PRESCALER_16 
 * - SPI_PRESCALER_32 
 * - SPI_PRESCALER_64 
 * - SPI_PRESCALER_128
*/
#define SPI_CFG_PRESCALER           SPI_PRESCALER_128

/******************** END OF USER CONFIGURATIONS ********************/

#endif /* INC_SPI_CFG_H_ */
