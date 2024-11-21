#include "Debug_Driver.h"
                
void Debug_Led(uint8 color[10],boolean state)
{
	if(strcmp(color ,"Red")==0)
	{
		if(state == 1)
		    Dio_WriteChannel(DioConf_LED1_CHANNEL_NUM, STD_HIGH);
		else
		    Dio_WriteChannel(DioConf_LED1_CHANNEL_NUM, STD_LOW);
	}
	
		else if(strcmp(color ,"Blue")==0)
	{
		if(state == 1)
		    Dio_WriteChannel(DioConf_LED2_CHANNEL_NUM, STD_HIGH);
		else
		    Dio_WriteChannel(DioConf_LED2_CHANNEL_NUM, STD_LOW);
	}
	
			else if(strcmp(color ,"Green")==0)
	{
		if(state == 1)
		    Dio_WriteChannel(DioConf_LED3_CHANNEL_NUM, STD_HIGH);
		else
		    Dio_WriteChannel(DioConf_LED3_CHANNEL_NUM, STD_LOW);
	}

}
void Debug_Intialization(void)
{
    /*
  CLK_Enable('F');															//Enable clk For Port F ()
  GPIO_PORT_UNLOCK('F');													//Unlock Port F ()
        
  PIN_ANALOG_DIGITAL('F',1,"Digital");										//Set pin F1 as Digital ()
  PIN_FUNCTION_SELECT('F',1,"General I/O");									//Set pin F1 as general input/output ()
  PIN_INPUT_OUTPUT('F',1,"Output");											//Set pin F1 as Output

  PIN_ANALOG_DIGITAL('F',2,"Digital");										//Set pin F2 as Digital
  PIN_FUNCTION_SELECT('F',2,"General I/O");									//Set pin F2 as general input/output
  PIN_INPUT_OUTPUT('F',2,"Output");											//Set pin F2 as Output
	
  PIN_ANALOG_DIGITAL('F',3,"Digital");										//Set pin F3 as Digital
  PIN_FUNCTION_SELECT('F',3,"General I/O");									//Set pin F3 as general input/output
  PIN_INPUT_OUTPUT('F',3,"Output");											//Set pin F3 as Output
  
  */

  Debug_Led("Red",1);														//Turn off Red Led
  Debug_Led("Blue",1);														//Turn off Blue Led
  Debug_Led("Green",1);														//Turn off Green Led
}
