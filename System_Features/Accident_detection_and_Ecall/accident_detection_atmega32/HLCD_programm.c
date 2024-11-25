/************************************/
/*  Author 	   : MAHMOUD SAMIR	    */
/*	SWC		   : LCD                */
/*  Layer 	   : HAL				*/
/*  Date	   : Oct. 1		2023	*/
/*  version    : 1.0				*/
/*  Last Edit  : N/A				*/
/************************************/

#define F_CPU 8000000UL
#include "util/delay.h"

#include "LSTD_types.h"
#include "LBIT_math.h"


#include "MDIO_interface.h"
#include "HLCD_interface.h"
#include "HLCD_config.h"

void HLCD_voidInit(void)
{
	MDIO_voidSetPortDirection(HLCD_DATA_PORT,0xff);
	MDIO_voidSetPinDirection(HLCD_CTRL_PORT,HLCD_RS_PIN,MDIO_OUTPUT);
	MDIO_voidSetPinDirection(HLCD_CTRL_PORT,HLCD_RW_PIN,MDIO_OUTPUT);
	MDIO_voidSetPinDirection(HLCD_CTRL_PORT,HLCD_EN_PIN,MDIO_OUTPUT);
	_delay_ms(30);
	HLCD_voidWriteCmd(0x3f);
	_delay_us(50);
	HLCD_voidWriteCmd(0x0f);
	_delay_us(50);
	HLCD_voidClearScreen();
	HLCD_voidWriteCmd(0x06);
	_delay_us(50);
}

void HLCD_voidWriteChar(const char ARG_ccharChar)
{
	MDIO_voidSetPinValue(HLCD_CTRL_PORT,HLCD_RW_PIN,MDIO_LOW);
	MDIO_voidSetPinValue(HLCD_CTRL_PORT,HLCD_RS_PIN,MDIO_HIGH);
	MDIO_voidSetPortValue(HLCD_DATA_PORT,ARG_ccharChar);
	MDIO_voidSetPinValue(HLCD_CTRL_PORT,HLCD_EN_PIN,MDIO_LOW);
	MDIO_voidSetPinValue(HLCD_CTRL_PORT,HLCD_EN_PIN,MDIO_HIGH);
	_delay_us(1);
	MDIO_voidSetPinValue(HLCD_CTRL_PORT,HLCD_EN_PIN,MDIO_LOW);
	_delay_us(1);
}

void HLCD_voidWriteCmd(const u8 ARG_cu8Cmd)
{
	MDIO_voidSetPinValue(HLCD_CTRL_PORT,HLCD_RW_PIN,MDIO_LOW);
	MDIO_voidSetPinValue(HLCD_CTRL_PORT,HLCD_RS_PIN,MDIO_LOW);
	MDIO_voidSetPortValue(HLCD_DATA_PORT,ARG_cu8Cmd);
	MDIO_voidSetPinValue(HLCD_CTRL_PORT,HLCD_EN_PIN,MDIO_LOW);
	MDIO_voidSetPinValue(HLCD_CTRL_PORT,HLCD_EN_PIN,MDIO_HIGH);
	_delay_us(1);
	MDIO_voidSetPinValue(HLCD_CTRL_PORT,HLCD_EN_PIN,MDIO_LOW);
	_delay_us(1);

}

void HLCD_voidClearScreen(void)
{
	HLCD_voidWriteCmd(0x01);
	_delay_ms(2);
}

void HLCD_voidWriteString(const char* ARG_ccharpString)
{
	u8 L_u8i=0;
	do
	{
		HLCD_voidWriteChar(ARG_ccharpString[L_u8i]);
		L_u8i++;
	}
	while(ARG_ccharpString[L_u8i]!='\0');
}

void HLCD_voidSetCursor(u8 ARG_u8Row, u8 ARG_u8Col)
{
	if((ARG_u8Row<=1)&&(ARG_u8Col<=15))
	{
		HLCD_voidWriteCmd((1<<7)|(ARG_u8Row<<6)|ARG_u8Col);
	}
	else
	{
		/*Report an error*/
	}
}
void HLCD_voidintgerToString(int data)
{
   char buff[16]; /* String to hold the ascii result */
   itoa(data,buff,10); /* Use itoa C function to convert the data to its corresponding ASCII value, 10 for decimal */
   HLCD_voidWriteString(buff); /* Display the string */
}
