/*
 * Adc_PbCfg.c
 *
 *  Created on: Sep 30, 2023
 *      Author: ahmad
 */

#include "Adc.h"

Adc_ConfigurationType Adc_Configuration =
{
    .autoTrigger = ADC_AUTO_TRIGGER_OFF,
    .reference   = ADC_REF_AVCC,
    .prescaler   = 128
};
