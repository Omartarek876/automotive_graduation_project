/*************************************/
/* Author       : Mahmoud Samir  	 */
/* SWC          : Bit Math           */
/* Layer        : Library            */
/* Version      : 1.0                */
/* DATE         : September 9 2023   */
/*Last_Edit     : N/A                */
/*************************************/

#ifndef _LBIT_MATH_H_
#define _LBIT_MATH_H_

#define   SET_BIT(REG,BIT)    REG|=(1<<BIT)
#define   TOGGLE_BIT(REG,BIT) REG^=(1<<BIT)
#define   CLEAR_BIT(REG,BIT)  REG&=~(1<<BIT)
#define   GET_BIT(REG,BIT)    ((REG>>BIT)&1)

#endif // _LBIT_MATH_H_


