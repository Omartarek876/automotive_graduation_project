/*
 * I2c.c
 *
 *  Created on: Oct 27, 2023
 *      Author: omar tarek
 */
#include "StdTypes.h"
#include "Macros.h"
#include "Registers.h"
#include "Dio.h"
#include "I2c_Cfg.h"
#include "I2c.h"

#define I2C_GET_STATUS()   (TWSR&0xF8)

void I2c_Init(void)
{
#if (I2C_CFG_MODE == I2C_MASTER)
    /* Set TWBR = 2, TWPS = 0 -> SCL Frequency = 400kHz*/
    TWBR = 2;
    CLR_BIT(TWSR, 0);
    CLR_BIT(TWSR, 1);
#endif
    /* Device Address*/
    TWAR = I2C_CFG_DEVICE_ADDRESS<<1;
    /* General Call Recognition Enable */
#if (I2C_CFG_GENERAL_CALL_RECOG == I2C_ON)
    SET_BIT(TWAR, 0);
#endif
    /* Enable Pull Up Resistor */
    Dio_SetPinMode(I2C_PIN_SDA, DIO_MODE_INPUT_PULLUP);
    Dio_SetPinMode(I2C_PIN_SCL, DIO_MODE_INPUT_PULLUP);
    /* TWI Enable Bit */
    SET_BIT(TWCR, 2);
}

I2c_StatusType I2c_SendStart(void)
{
    /* Set Start Condition Bit */
    SET_BIT(TWCR, 5);
    /* Wait for Flag */
    while (GET_BIT(TWCR, 7) == 0);
    /* Return Status Code */
    return I2C_GET_STATUS();
}

I2c_StatusType I2c_SendSlaveAddress(u8 address, I2c_RequestType request)
{
    /* Write SA + Request*/
    TWDR = address<<1 | (request&1);
    /* Clear Start Condition Bit and Flag */
    TWCR &= ~((1<<5) | (1<<7));
    /* Wait for Flag */
    while (GET_BIT(TWCR, 7) == 0);
    /* Return Status Code */
    return I2C_GET_STATUS();
}

I2c_StatusType I2c_WriteData(u8 data)
{
    /* Write Data */
    TWDR = data;
    /* Clear Flag */
    CLR_BIT(TWCR, 7);
    /* Wait for Flag */
    while (GET_BIT(TWCR, 7) == 0);
    /* Return Status Code */
    return I2C_GET_STATUS();
}

I2c_StatusType I2c_ReadData(u8* data)
{
    /* Clear Flag */
    CLR_BIT(TWCR, 7);
    /* Wait for Flag */
    while (GET_BIT(TWCR, 7) == 0);
    /* Read Data */
    *data = TWDR;
    /* Return Status Code */
    return I2C_GET_STATUS();
}

void I2c_SendStop(void)
{
    /* Clear Flag, Set Stop bit, Enable TWI*/
    TWCR = (1<<7) | (1<<4) | (1<<2);
}

void I2c_EnableAck(void)
{
    SET_BIT(TWCR, 6);
}

void I2c_DisableAck(void)
{
    CLR_BIT(TWCR, 6);
}

I2c_ErrorType I2c_MasterTransmit(u8 slaveAddress, u8* dataBuffer, u8 length)
{
    I2c_StatusType status;
    status = I2c_SendStart();
    if (I2C_STATUS_MASTER_START_SENT != status)
    {
        return I2C_ERROR_START_NOT_SENT;
    }
    status = I2c_SendSlaveAddress(slaveAddress, I2C_REQ_WRITE);
    if (I2C_STATUS_MASTER_SA_W_ACK != status)
    {
        I2c_SendStop();
        return I2C_ERROR_SLAVE_NOT_RESPOND;
    }
    while (length > 0)
    {
        status = I2c_WriteData(*dataBuffer);
        if (I2C_STATUS_MASTER_DATA_SENT_ACK != status)
        {
            I2c_SendStop();
            return I2C_ERROR_DATA_NOT_SENT;
        }
        length--;
        dataBuffer++;
    }
    I2c_SendStop();
    return I2C_NO_ERROR;
}

I2c_ErrorType I2c_SlaveReceive(u8* dataBuffer, u8 length)
{
    I2c_EnableAck();

    /* Wait for Flag */
    while (GET_BIT(TWCR, 7) == 0);
    if (I2C_STATUS_SLAVE_SA_W_RECEIVED != I2C_GET_STATUS())
    {
        I2c_DisableAck();
        /* Clear Flag */
        SET_BIT(TWCR, 7);
        return I2C_ERROR_SA_W_NOT_RECEIVED;
    }
    while (length > 0)
    {
        if (I2C_STATUS_SLAVE_SA_DATA_RECEIVED_ACK != I2c_ReadData(dataBuffer))
        {
            I2c_DisableAck();
            /* Clear Flag */
            SET_BIT(TWCR, 7);
            return I2C_ERROR_DATA_NOT_RECEIVED;

        }
        dataBuffer++;
        length--;
    }
    /* Clear Flag */
    SET_BIT(TWCR, 7);
    I2c_DisableAck();
    return I2C_NO_ERROR;
}