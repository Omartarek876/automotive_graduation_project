/*************************************/
/* Author       : Mahmoud Samir  	 */
/* SWC          : DIO                */
/* Layer        : MCAL               */
/* Version      : 1.0                */
/* DATE         : September 9 2023   */
/*Last_Edit     : N/A                */
/*************************************/
#ifndef MDIO_INTERFACE_H
#define MDIO_INTERFACE_H

#define MDIO_PORTA 0
#define MDIO_PORTB 1
#define MDIO_PORTC 2
#define MDIO_PORTD 3

#define MDIO_PORT_OUTPUT 0xff
#define MDIO_PORT_INPUT  0x00

#define MDIO_INPUT 10
#define MDIO_OUTPUT 11
#define MDIO_INPUT_PULLUP 12

#define MDIO_LOW  	0
#define MDIO_HIGH	1

		//Port Access//
void MDIO_voidSetPortDirection(u8 ARG_u8Port,u8 ARG_u8Direction);
void MDIO_voidSetPortValue(u8 ARG_u8Port,u8 ARG_u8Value );
u8 	 MDIO_u8GetPortValue(u8 ARG_u8Port);

		//Pin Access//
void MDIO_voidSetPinDirection(u8 ARG_u8Port,u8 ARG_u8Pin,u8 ARG_u8Direction);
void MDIO_voidSetPinValue(u8 ARG_u8Port,u8 ARG_u8Pin,u8 ARG_u8Value);
u8 MDIO_u8GetPinValue(u8 ARG_u8Port,u8 ARG_u8Pin);


#endif

