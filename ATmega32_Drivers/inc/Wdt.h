/*
 * Wdt.h
 *
 *  Created on: Oct 14, 2023
 *      Author: omar tarek
 */

#ifndef INC_WDT_H_
#define INC_WDT_H_

typedef enum 
{
    WDT_TIMEOUT_16_3_MS,
    WDT_TIMEOUT_32_5_MS,
    WDT_TIMEOUT_65_MS,
    WDT_TIMEOUT_130_MS,
    WDT_TIMEOUT_260_MS,
    WDT_TIMEOUT_520_MS,
    WDT_TIMEOUT_1000_MS,
    WDT_TIMEOUT_2100_MS,
} Wdt_TimeoutType;

void Wdt_Enable(void);
void Wdt_Disable(void);
void Wdt_Refresh(Wdt_TimeoutType timeout);

#endif /* INC_WDT_H_ */
