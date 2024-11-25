#include "LSTD_TYPES.h"
#include "LBIT_MATH.h"
#include "stdlib.h"
#include <util/delay.h>
#include "i2C.h"
#include "MDIO_interface.h"
#include"HLCD_interface.h"

#define MPU6050_ADDRESS       0x68
#define MPU6050_PWR_MGMT      0x6B
#define MPU6050_ACCEL_CONFIG  0x1C
#define MPU6050_ACC_START     0x3B

#define ACCEL_SENSITIVITY    16384.0
#define GRAVITY               9.8

void MPU6050_Init(void) {
    I2c_Init();
    _delay_ms(100);

    I2c_SendStart();
    I2c_SendSlaveAddress(MPU6050_ADDRESS, I2C_REQ_WRITE);
    I2c_WriteData(MPU6050_PWR_MGMT);
    I2c_WriteData(0x00);
    I2c_SendStop();

    I2c_SendStart();
    I2c_SendSlaveAddress(MPU6050_ADDRESS, I2C_REQ_WRITE);
    I2c_WriteData(MPU6050_ACCEL_CONFIG);
    I2c_WriteData(0x00);
    I2c_SendStop();
}

void MPU6050_ReadAccel(float* ax, float* ay, float* az) {
    u8 data[6];
    u16 raw_ax, raw_ay, raw_az;

    I2c_SendStart();
    I2c_SendSlaveAddress(MPU6050_ADDRESS, I2C_REQ_WRITE);
    I2c_WriteData(MPU6050_ACC_START);
    I2c_SendStop();

    I2c_SendStart();
    I2c_SendSlaveAddress(MPU6050_ADDRESS, I2C_REQ_READ);
    for (u8 i = 0; i < 6; i++) {
        I2c_ReadData(&data[i]);
    }
    I2c_SendStop();

    raw_ax = (s16)((data[0] << 8) | data[1]);
    raw_ay = (s16)((data[2] << 8) | data[3]);
    raw_az = (s16)((data[4] << 8) | data[5]);

    *ax = (raw_ax / ACCEL_SENSITIVITY) * GRAVITY;
    *ay = (raw_ay / ACCEL_SENSITIVITY) * GRAVITY;
    *az = (raw_az / ACCEL_SENSITIVITY) *GRAVITY;
}

int main(void) {
    float ax, ay, az;
    MPU6050_Init();
    HLCD_voidInit();
    MDIO_voidSetPinDirection(MDIO_PORTB, 5, MDIO_OUTPUT);

    while (1) {
        MPU6050_ReadAccel(&ax, &ay, &az);
        HLCD_voidSetCursor(0,0);
              HLCD_voidWriteString("az= ");
              HLCD_voidintgerToString(az);

              HLCD_voidSetCursor(1,0);
              HLCD_voidWriteString("ay= ");
              HLCD_voidintgerToString(ay);
              _delay_ms(1000);
              HLCD_voidClearScreen();
/*
        if (az < 3.0) {
            MDIO_voidSetPinValue(MDIO_PORTC, 5, MDIO_HIGH);
            _delay_ms(500);
            MDIO_voidSetPinValue(MDIO_PORTB, 5, MDIO_LOW);
        } else {
            MDIO_voidSetPinValue(MDIO_PORTB, 5, MDIO_LOW);
        }
*/
        _delay_ms(500);
    }

    return 0;
}
