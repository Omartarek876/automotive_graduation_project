/************************************/
/*  Author 	   : MAHMOUD SAMIR  	*/
/*	SWC		   : LCD                */
/*  Layer 	   : HAL				*/
/*  Date	   : Oct. 1		2023	*/
/*  version    : 1.0				*/
/*  Last Edit  : N/A				*/
/************************************/

#ifndef _HLCD_INTERFACE_H_
#define _HLCD_INTERFACE_H_

void HLCD_voidInit(void);
void HLCD_voidWriteChar(const char ARG_ccharChar);
void HLCD_voidWriteCmd(const u8 ARG_cu8Cmd);
void HLCD_voidClearScreen(void);
void HLCD_voidWriteString(const char* ARG_ccharpString);
void HLCD_voidSetCursor(u8 ARG_u8Row, u8 ARG_u8Col);
void HLCD_voidintgerToString(int data);

#endif
