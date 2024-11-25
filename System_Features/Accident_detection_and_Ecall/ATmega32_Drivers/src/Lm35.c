/*
 * Lm35.c
 *
 *  Created on: Sep 30, 2023
 *      Author: omar tarek
 */
#include "StdTypes.h"
#include "Adc.h"
#include "Delay.h"
#include "Lm35.h"

u8 Lm35_GetTemperature (Adc_ChannelType adcChannel,u8 avgCount, u8 delayMs)
{
    u8 i;
    u32 result = 0;
    for (i=0; i<avgCount; i++)
    {
        Adc_StartConversion(adcChannel);
        result += Adc_GetResult();
        _delay_ms(delayMs);
    }
    result /= avgCount;
    return ((result*500)/1024);
}

