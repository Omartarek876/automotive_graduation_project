/*
 * Dio.h
 *
 *  Created on: Sep 15, 2023
 *      Author: omar tarek
 */

#ifndef INC_DIO_H_
#define INC_DIO_H_


typedef enum
{
    DIO_PORTA,
    DIO_PORTB,
    DIO_PORTC,
    DIO_PORTD
} Dio_PortType;

typedef enum
{
    DIO_PIN0,
    DIO_PIN1,
    DIO_PIN2,
    DIO_PIN3,
    DIO_PIN4,
    DIO_PIN5,
    DIO_PIN6,
    DIO_PIN7
} Dio_PinType;

typedef enum
{
    DIO_MODE_OUTPUT,
    DIO_MODE_INPUT_FLOATING,
    DIO_MODE_INPUT_PULLUP
} Dio_ModeType;

typedef enum
{
    DIO_LOW,
    DIO_HIGH
} Dio_LevelType;

/**
 * @brief The function sets the mode of pin (input pullup/floating or output)
 * @param port: variable to hold port of pin of type (Dio_PortType)
 * @param pin: variable to hold pin index of type (Dio_PinType)
 * @param mode: variable to hold pin mode of type (Dio_ModeType)
 * @return The function return
 *          (E_OK) if it was able to set the pin mode 
 *          (E_NOT_OK) if the parameters are not correct
*/
StdReturnType Dio_SetPinMode (Dio_PortType port, Dio_PinType pin, Dio_ModeType mode);

/**
 * @brief The function sets the level of pin (high / low)
 * @param port: variable to hold port of pin of type (Dio_PortType)
 * @param pin: variable to hold pin index of type (Dio_PinType)
 * @param level: variable to hold pin level of type (Dio_LevelType)
 * @return The function return
 *          (E_OK) if it was able to set the pin level 
 *          (E_NOT_OK) if the parameters are not correct
*/
StdReturnType Dio_SetPinLevel(Dio_PortType port, Dio_PinType pin, Dio_LevelType level);

/**
 * @brief The function flips the level of pin (high / low)
 * @param port: variable to hold port of pin of type (Dio_PortType)
 * @param pin: variable to hold pin index of type (Dio_PinType)
 * @return The function return new pin level
*/
Dio_LevelType Dio_FlipPinLevel(Dio_PortType port, Dio_PinType pin);

/**
 * @brief The function reads the level of pin (high / low)
 * @param port: variable to hold port of pin of type (Dio_PortType)
 * @param pin: variable to hold pin index of type (Dio_PinType)
 * @return The function return pin level
*/
Dio_LevelType Dio_ReadPinLevel(Dio_PortType port, Dio_PinType pin);

#endif /* INC_DIO_H_ */
