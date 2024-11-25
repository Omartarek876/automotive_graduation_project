/*
 * Spi.c
 *
 *  Created on: Oct 21, 2023
 *      Author: omar tarek
 */
#include "StdTypes.h"
#include "Macros.h"
#include "Registers.h"
#include "Interrupts.h"
#include "Dio.h"
#include "Spi_Cfg.h"
#include "Spi.h"

static void (*Callback_Spi) (u8 data) = NULL_PTR;

ISR(VECTOR_SPI)
{
    if (NULL_PTR != Callback_Spi)
    {
        Callback_Spi(SPDR);
    }
}

void Spi_Init(void)
{
    /* Master/Slave Select */
#if (SPI_CFG_MODE == SPI_MASTER)
    Dio_SetPinMode(SPI_PIN_MOSI, DIO_MODE_OUTPUT);
    Dio_SetPinMode(SPI_PIN_MISO, DIO_MODE_INPUT_FLOATING);
    Dio_SetPinMode(SPI_PIN_SCK, DIO_MODE_OUTPUT);
    Dio_SetPinMode(SPI_PIN_SS, DIO_MODE_OUTPUT);
    SET_BIT(SPCR, 4);
#elif (SPI_CFG_MODE == SPI_SLAVE)
    Dio_SetPinMode(SPI_PIN_MOSI, DIO_MODE_INPUT_FLOATING);
    Dio_SetPinMode(SPI_PIN_MISO, DIO_MODE_OUTPUT);
    Dio_SetPinMode(SPI_PIN_SCK, DIO_MODE_INPUT_FLOATING);
    Dio_SetPinMode(SPI_PIN_SS, DIO_MODE_INPUT_FLOATING);
    CLR_BIT(SPCR, 4);
#else
#error "Invalid SPI_CFG_MODE"
#endif

    /* Data Order */
#if (SPI_CFG_DATA_ORDER == SPI_LSB_FIRST)
    SET_BIT(SPCR, 5);
#elif (SPI_CFG_DATA_ORDER == SPI_MSB_FIRST)
    CLR_BIT(SPCR, 5);
#else 
#error "Invalid SPI_CFG_DATA_ORDER"
#endif

    /* Clock Polarity */
#if (SPI_CFG_CLOCK_POLARITY == SPI_SCK_IDLE_LOW)
    CLR_BIT(SPCR, 3);
#elif (SPI_CFG_CLOCK_POLARITY == SPI_SCK_IDLE_HIGH)
    SET_BIT(SPCR, 3);
#else 
#error "Invalid SPI_CFG_CLOCK_POLARITY"
#endif

    /* Clock Phase */
#if (SPI_CFG_CLOCK_PHASE == SPI_SAMPLE_THEN_SETUP)
    CLR_BIT(SPCR, 2);
#elif (SPI_CFG_CLOCK_PHASE == SPI_SETUP_THEN_SAMPLE)
    SET_BIT(SPCR, 2);
#else 
#error "Invalid SPI_CFG_CLOCK_PHASE"
#endif

    /* Prescaler */
#if (SPI_CFG_PRESCALER == SPI_PRESCALER_2)
    CLR_BIT(SPCR, 0);
    CLR_BIT(SPCR, 1);
    SET_BIT(SPSR, 0);
#elif (SPI_CFG_PRESCALER == SPI_PRESCALER_4)
    CLR_BIT(SPCR, 0);
    CLR_BIT(SPCR, 1);
    CLR_BIT(SPSR, 0);
#elif (SPI_CFG_PRESCALER == SPI_PRESCALER_8)
    SET_BIT(SPCR, 0);
    CLR_BIT(SPCR, 1);
    SET_BIT(SPSR, 0);
#elif (SPI_CFG_PRESCALER == SPI_PRESCALER_16)
    SET_BIT(SPCR, 0);
    CLR_BIT(SPCR, 1);
    CLR_BIT(SPSR, 0);
#elif (SPI_CFG_PRESCALER == SPI_PRESCALER_32)
    CLR_BIT(SPCR, 0);
    SET_BIT(SPCR, 1);
    SET_BIT(SPSR, 0);
#elif (SPI_CFG_PRESCALER == SPI_PRESCALER_64)
    CLR_BIT(SPCR, 0);
    SET_BIT(SPCR, 1);
    CLR_BIT(SPSR, 0);
#elif (SPI_CFG_PRESCALER == SPI_PRESCALER_128)
    SET_BIT(SPCR, 0);
    SET_BIT(SPCR, 1);
    CLR_BIT(SPSR, 0);
#else 
#error "Invalid SPI_CFG_PRESCALER"
#endif

    /* SPI Enable */
    SET_BIT(SPCR, 6);
}

u8 Spi_Transfer(u8 data)
{
    SPDR = data;
    while(GET_BIT(SPSR, 7) == 0);
    return SPDR;
}

void Spi_EnableInterrupt(void)
{
    SET_BIT(SPCR, 7);
}

void Spi_DisableInterrupt(void)
{
    CLR_BIT(SPCR, 7);
}

void Spi_WriteData(u8 data)
{
    SPDR = data;
}

void Spi_SetCallback(void (*callbackPtr)(u8 data))
{
    Callback_Spi = callbackPtr;
}

