/*
 * I2c_Cfg.h
 *
 *  Created on: Oct 27, 2023
 *      Author: omar tarek
 */

#ifndef INC_I2C_CFG_H_
#define INC_I2C_CFG_H_

#define I2C_MASTER  1
#define I2C_SLAVE   2
#define I2C_OFF     0
#define I2C_ON      1

#define I2C_CFG_MODE                I2C_MASTER
#define I2C_CFG_DEVICE_ADDRESS      0x20
#define I2C_CFG_GENERAL_CALL_RECOG  I2C_OFF

#endif /* INC_I2C_CFG_H_ */
