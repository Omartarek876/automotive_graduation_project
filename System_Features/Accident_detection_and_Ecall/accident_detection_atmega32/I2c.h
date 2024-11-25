/*
 * I2c.h
 *
 *  Created on: Oct 27, 2023
 *      Author: omar tarek
 */

#ifndef INC_I2C_H_
#define INC_I2C_H_

#include "LSTD_TYPES.h"
//#define I2C_PIN_SDA     DIO_PORTC,DIO_PIN1
//#define I2C_PIN_SCL     DIO_PORTC,DIO_PIN0

/************* I2C Registers *************/
#define TWBR        *((volatile u8*) 0x20)
#define TWCR		*((volatile u8*) 0x56)
#define TWSR		*((volatile u8*) 0x21)
#define TWDR		*((volatile u8*) 0x23)
#define TWAR		*((volatile u8*) 0x22)


typedef enum
{

    /************************************************************************
     * Master Status
     ***********************************************************************/

    /* A START condition has been transmitted */
    I2C_STATUS_MASTER_START_SENT = 0x08,

    /* A repeated START condition has been transmitted */
    I2C_STATUS_MASTER_REP_START_SENT = 0x10,

    /* SLA+W has been transmitted; ACK has been received */
    I2C_STATUS_MASTER_SA_W_ACK = 0x18,

    /* SLA+W has been transmitted; NOT ACK has been received */
    I2C_STATUS_MASTER_SA_W_NACK = 0x20,

    /* SLA+R has been transmitted; ACK has been received */
    I2C_STATUS_MASTER_SA_R_ACK = 0x40,

    /* SLA+R has been transmitted; NOT ACK has been received */
    I2C_STATUS_MASTER_SA_R_NACK = 0x48,

    /* Data byte has been transmitted; ACK has been received */
    I2C_STATUS_MASTER_DATA_SENT_ACK = 0x28,

    /* Data byte has been transmitted; NOT ACK has been received */
    I2C_STATUS_MASTER_DATA_SENT_NACK = 0x30,

    /* Data byte has been received; ACK has been returned */
    I2C_STATUS_MASTER_DATA_RECEIVED_ACK = 0x50,

    /* Data byte has been received; NOT ACK has been returned */
    I2C_STATUS_MASTER_DATA_RECEIVED_NACK = 0x58,

    /* Arbitration lost */
    I2C_STATUS_MASTER_ARBITRATION_LOST = 0x38,

    /************************************************************************
     * Slave Status
     ***********************************************************************/

    /* Own SLA+W has been received; ACK has been returned */
    I2C_STATUS_SLAVE_SA_W_RECEIVED = 0x60,

    /* Own SLA+R has been received; ACK has been returned */
    I2C_STATUS_SLAVE_SA_R_RECEIVED = 0xA8,

    /* General call address has been received; ACK has been returned */
    I2C_STATUS_SLAVE_GC_RECEIVED = 0x70,

    /* Previously addressed with own SLA+W; data has been received; ACK has been returned */
    I2C_STATUS_SLAVE_SA_DATA_RECEIVED_ACK = 0x80,

    /* Previously addressed with own SLA+W; data has been received; NOT ACK has been returned */
    I2C_STATUS_SLAVE_SA_DATA_RECEIVED_NACK = 0x88,

    /* Previously addressed with general call; data has been received; ACK has been returned */
    I2C_STATUS_SLAVE_GC_DATA_RECEIVED_ACK = 0x90,

    /* Previously addressed with general call; data has been received; NOT ACK has been returned */
    I2C_STATUS_SLAVE_GC_DATA_RECEIVED_NACK = 0x98,

    /* Last data byte in TWDR has been transmitted (TWEA = â€œ0â€�); ACK has been received */
    I2C_STATUS_SLAVE_LAST_DATA_SENT_ACK = 0xC8,

    /* Data byte in TWDR has been transmitted; ACK has been received */
    I2C_STATUS_SLAVE_DATA_SENT_ACK = 0xB8,

    /* Data byte in TWDR has been transmitted; NOT ACK has been received */
    I2C_STATUS_SLAVE_DATA_SENT_NACK = 0xC0,

    /* A STOP condition or repeated START condition has been received while still addressed as slave */
    I2C_STATUS_SLAVE_STOP_OR_REP_START_RECEIVED = 0xA0

} I2c_StatusType;


typedef enum 
{
    I2C_NO_ERROR,
    I2C_ERROR_START_NOT_SENT,
    I2C_ERROR_SLAVE_NOT_RESPOND,
    I2C_ERROR_DATA_NOT_SENT,
    I2C_ERROR_SA_W_NOT_RECEIVED,
    I2C_ERROR_DATA_NOT_RECEIVED,
} I2c_ErrorType;

typedef enum 
{
    I2C_REQ_WRITE = 0,
    I2C_REQ_READ = 1
} I2c_RequestType;

void I2c_Init(void);
I2c_StatusType I2c_SendStart(void);
I2c_StatusType I2c_SendSlaveAddress(u8 address, I2c_RequestType request);
I2c_StatusType I2c_WriteData(u8 data);
I2c_StatusType I2c_ReadData(u8* data);
void I2c_SendStop(void);
void I2c_EnableAck(void);
void I2c_DisableAck(void);

I2c_ErrorType I2c_MasterTransmit(u8 slaveAddress, u8* dataBuffer, u8 length);
I2c_ErrorType I2c_SlaveReceive(u8* dataBuffer, u8 length);


#endif /* INC_I2C_H_ */
