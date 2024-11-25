/*
 * Lm35.h
 *
 *  Created on: Sep 30, 2023
 *      Author: omar tarek
 */

#ifndef INC_LM35_H_
#define INC_LM35_H_

#include "StdTypes.h"
#include "Adc.h"

u8 Lm35_GetTemperature (Adc_ChannelType adcChannel,u8 avgCount, u8 delayMs);

#endif /* INC_LM35_H_ */
