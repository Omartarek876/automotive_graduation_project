/*************************************/
/* Author       : Mahmoud Samir  	 */
/* SWC          : DIO                */
/* Layer        : MCAL               */
/* Version      : 1.0                */
/* DATE         : September 9 2023   */
/*Last_Edit     : N/A                */
/*************************************/


#ifndef _MDIO_PRIVATE_H
#define _MDIO_PRIVATE_H


typedef struct {
    const u8 PIN;
    u8       DDR;
    u8       PORT;
}GPIO_t;

#define GPIOA ((volatile GPIO_t*)0x39)
#define GPIOB ((volatile GPIO_t*)0x36)
#define GPIOC ((volatile GPIO_t*)0x33)
#define GPIOD ((volatile GPIO_t*)0x30)

#endif // _MDIO_PRIVATE_H
