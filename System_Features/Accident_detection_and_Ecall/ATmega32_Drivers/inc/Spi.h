/*
 * Spi.h
 *
 *  Created on: Oct 21, 2023
 *      Author: omar tarek
 */

#ifndef INC_SPI_H_
#define INC_SPI_H_

#include "StdTypes.h"
#include "Dio.h"

#define SPI_PIN_SS      DIO_PORTB,DIO_PIN4
#define SPI_PIN_MOSI    DIO_PORTB,DIO_PIN5
#define SPI_PIN_MISO    DIO_PORTB,DIO_PIN6
#define SPI_PIN_SCK     DIO_PORTB,DIO_PIN7

void Spi_Init(void);
u8 Spi_Transfer(u8 data);
void Spi_EnableInterrupt(void);
void Spi_DisableInterrupt(void);
void Spi_WriteData(u8 data);
void Spi_SetCallback(void (*callbackPtr)(u8 data));

#endif /* INC_SPI_H_ */
