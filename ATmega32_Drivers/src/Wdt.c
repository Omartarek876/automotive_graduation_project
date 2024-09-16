/*
 * Wdt.c
 *
 *  Created on: Oct 14, 2023
 *      Author: omar tarek
 */
#include "StdTypes.h"
#include "Registers.h"
#include "Macros.h"
#include "Wdt.h"

void Wdt_Enable(void)
{
    SET_BIT(WDTCR, 3);
}

void Wdt_Disable(void)
{
    /* In the same operation, write a logic one to WDTOE and WDE. */
    WDTCR |= (1<<3) | (1<<4);
    /* Within the next four clock cycles, write a logic 0 to WDE. */
    WDTCR &= 0b11110111;
}

void Wdt_Refresh(Wdt_TimeoutType timeout)
{
    /* Set WDG Timer Timeout */
    WDTCR &= 0b11111000;
    WDTCR |= (timeout&0b111);
    /* Refresh WDG Timer */
    Wdt_Disable();
    Wdt_Enable();
}
