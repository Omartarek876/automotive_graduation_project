/*************************************/
/* Author       : Mahmoud Samir  	 */
/* SWC          : DIO                */
/* Layer        : MCAL               */
/* Version      : 1.0                */
/* DATE         : September 9 2023   */
/*Last_Edit     : N/A                */
/*************************************/

#include "LSTD_TYPES.h"
#include "LBIT_MATH.h"

#include "MDIO_interface.h"
#include "MDIO_private.h"


void MDIO_voidSetPortDirection(u8 ARG_u8Port,u8 ARG_u8Direction)
{
	switch(ARG_u8Port)
	{
		case MDIO_PORTA : GPIOA -> DDR = ARG_u8Direction; break;
		case MDIO_PORTB : GPIOB -> DDR = ARG_u8Direction; break;
		case MDIO_PORTC : GPIOC -> DDR = ARG_u8Direction; break;
		case MDIO_PORTD : GPIOD -> DDR = ARG_u8Direction; break;
		default : /* Wrong case*/ break;
	}
}

void MDIO_voidSetPortValue(u8 ARG_u8Port,u8 ARG_u8Value )
{
	switch(ARG_u8Port)
	{
		case MDIO_PORTA : GPIOA -> PORT = ARG_u8Value; break;
		case MDIO_PORTB : GPIOB -> PORT = ARG_u8Value; break;
		case MDIO_PORTC : GPIOC -> PORT = ARG_u8Value; break;
		case MDIO_PORTD : GPIOD -> PORT = ARG_u8Value; break;
		default : /* Wrong case*/ break;
	}
}

u8 	 MDIO_u8GetPortValue(u8 ARG_u8Port)
{
	u8 L_u8Value=0;
	switch(ARG_u8Port)
	{
		case MDIO_PORTA : L_u8Value = GPIOA -> PIN ; break;
		case MDIO_PORTB : L_u8Value = GPIOB -> PIN ; break;
		case MDIO_PORTC : L_u8Value = GPIOC -> PIN ; break;
		case MDIO_PORTD : L_u8Value = GPIOD -> PIN ; break;
		default : /* Wrong case*/ break;
	}
	return L_u8Value;
}


void MDIO_voidSetPinDirection(u8 ARG_u8Port,u8 ARG_u8Pin,u8 ARG_u8Direction)
{
	if(ARG_u8Pin<=7)
	{
		if(MDIO_OUTPUT==ARG_u8Direction)
		{
			switch(ARG_u8Port)
			{
			case MDIO_PORTA: SET_BIT(GPIOA->DDR, ARG_u8Pin);break;
			case MDIO_PORTB: SET_BIT(GPIOB->DDR, ARG_u8Pin);break;
			case MDIO_PORTC: SET_BIT(GPIOC->DDR, ARG_u8Pin);break;
			case MDIO_PORTD: SET_BIT(GPIOD->DDR, ARG_u8Pin);break;
			default : /* Wrong case*/ break;
			}
		}
		else if(MDIO_INPUT==ARG_u8Direction)
		{
			switch(ARG_u8Port)
			{
			case MDIO_PORTA: CLEAR_BIT(GPIOA->DDR, ARG_u8Pin);break;
			case MDIO_PORTB: CLEAR_BIT(GPIOB->DDR, ARG_u8Pin);break;
			case MDIO_PORTC: CLEAR_BIT(GPIOC->DDR, ARG_u8Pin);break;
			case MDIO_PORTD: CLEAR_BIT(GPIOD->DDR, ARG_u8Pin);break;
			default : /* Wrong case*/ break;
			}
		}
		else if(MDIO_INPUT_PULLUP==ARG_u8Direction)
		{
			switch(ARG_u8Port)
			{
			case MDIO_PORTA: CLEAR_BIT(GPIOA->DDR, ARG_u8Pin);SET_BIT(GPIOA->PORT, ARG_u8Pin);break;
			case MDIO_PORTB: CLEAR_BIT(GPIOB->DDR, ARG_u8Pin);SET_BIT(GPIOB->PORT, ARG_u8Pin);break;
			case MDIO_PORTC: CLEAR_BIT(GPIOC->DDR, ARG_u8Pin);SET_BIT(GPIOC->PORT, ARG_u8Pin);break;
			case MDIO_PORTD: CLEAR_BIT(GPIOD->DDR, ARG_u8Pin);SET_BIT(GPIOD->PORT, ARG_u8Pin);break;
			default : /* Wrong case*/ break;
			}
		}
		else
		{
			/*Wrong case*/
		}
	}
	else
	{
		/*Wrong case*/
	}
}

void MDIO_voidSetPinValue(u8 ARG_u8Port,u8 ARG_u8Pin,u8 ARG_u8Value)
{
	if(ARG_u8Pin<=7)
		{
			if(MDIO_HIGH==ARG_u8Value)
			{
				switch(ARG_u8Port)
				{
				case MDIO_PORTA: SET_BIT(GPIOA->PORT, ARG_u8Pin);break;
				case MDIO_PORTB: SET_BIT(GPIOB->PORT, ARG_u8Pin);break;
				case MDIO_PORTC: SET_BIT(GPIOC->PORT, ARG_u8Pin);break;
				case MDIO_PORTD: SET_BIT(GPIOD->PORT, ARG_u8Pin);break;
				default : /* Wrong case*/ break;
				}
			}
			else if(MDIO_LOW==ARG_u8Value)
			{
				switch(ARG_u8Port)
				{
				case MDIO_PORTA: CLEAR_BIT(GPIOA->PORT, ARG_u8Pin);break;
				case MDIO_PORTB: CLEAR_BIT(GPIOB->PORT, ARG_u8Pin);break;
				case MDIO_PORTC: CLEAR_BIT(GPIOC->PORT, ARG_u8Pin);break;
				case MDIO_PORTD: CLEAR_BIT(GPIOD->PORT, ARG_u8Pin);break;
				default : /* Wrong case*/ break;
				}
			}
			else
			{
				/*Wrong case*/
			}
		}
		else
		{
			/*Wrong case*/
		}
}

u8 MDIO_u8GetPinValue(u8 ARG_u8Port,u8 ARG_u8Pin)
{
	u8 L_u8Value;
	if(ARG_u8Pin<=7)
	{
		switch(ARG_u8Port)
		{
		case MDIO_PORTA:L_u8Value= GET_BIT(GPIOA->PIN, ARG_u8Pin);break;
		case MDIO_PORTB:L_u8Value= GET_BIT(GPIOB->PIN, ARG_u8Pin);break;
		case MDIO_PORTC:L_u8Value= GET_BIT(GPIOC->PIN, ARG_u8Pin);break;
		case MDIO_PORTD:L_u8Value= GET_BIT(GPIOD->PIN, ARG_u8Pin);break;
		default : /* Wrong case*/ break;
		}
	}
	else
	{
		/*Wrong case*/
	}
	return L_u8Value;
}
