
/*
 * Timer.c
 *
 * Created on: 21/2/2025
 *      Author: Mariam
 */

#include "Timer0.h"

int i;


void Timer0_Delay(uint32 time)
{
    SET_BIT(RCGCTIMER,0);    //Enable clock for timer0

    CLEAR_BIT(GPTMCTL,0);      //Disable timer0 during configuration


    GPTMCFG=0x04;

    GPTMTAMR = 0x02;     //Select one shot timer& Down count selection

    GPTMTAILR = 16-1;  //Set clock to 16MHZ
    SET_BIT(GPTMICR,0);       //Clear flag

    SET_BIT(GPTMCTL,0);        //Enable timer0
    for(i=0;i<time;i++)
    {
        while((GET_BIT(GPTMRIS,0))==0); //wait for TimerA timeout flag to set*/
        SET_BIT(GPTMICR,0);       //Clear flag
    }
}

void Timer3_Count_Init(void){


    SET_BIT(RCGCTIMER,3);    //Enable clock for timer3
    SET_BIT(RCGCGPIO,1);        //enable GPIO clock

    CLEAR_BIT(GPIO_PORTB_DIR_REG,2);        //enable GPIO clock 3shan l echo
    SET_BIT(GPIO_PORTB_DEN_REG,2);        //enable GPIO clock
    SET_BIT(GPIO_PORTB_AFSEL_REG,2);        //enable GPIO clock

    GPIO_PORTB_PCTL_REG  &=~0x00000F00;
    GPIO_PORTB_PCTL_REG  |=0x00000700;

    /* PB2 as a digital output signal to provide trigger signal */

    SET_BIT(RCGCGPIO,1);        //enable GPIO clock

      CLEAR_BIT(GPIO_PORTA_DIR_REG,3);        //enable GPIO clock 3shan l echo
      SET_BIT(GPIO_PORTA_DEN_REG,3);        //enable GPIO clock


    CLEAR_BIT(GPTMCTL3,0);      //Disable timer0 during configuration


    GPTMCFG3=0x04;

    GPTMTAMR3 = 0x17;     //Select one shot timer& Down count selection
//    GPTAMATCHR3 = 0xFFFF;
  //  GPTMTAPMR3 = 0xFF;
    SET_BIT(GPTMCTL3, 2);
    SET_BIT(GPTMCTL3, 3);
    //clear 3

    SET_BIT(GPTMCTL3,0);        //Enable timer3 to start counting
}



/*
uint32 Timer3_Count_Ret(void){

    SET_BIT(GPTMICR3,2);
    while(GET_BIT(GPTMRIS3, 2)==0){}
    uint32 duration1 =  GPTMTAR3;


    SET_BIT(GPTMICR3,2);
    while(GET_BIT(GPTMRIS3, 2)==0){}
    uint32 duration2 =   GPTMTAR3;


    return (duration2 - duration1) &0x00FFFFFF;

}


 * #define GPIO_PORTB_DATA_REG       (*((volatile uint32 *)0x400053FC))
#define GPIO_PORTB_DIR_REG        (*((volatile uint32 *)0x40005400))
#define GPIO_PORTB_AFSEL_REG      (*((volatile uint32 *)0x40005420))
#define GPIO_PORTB_PUR_REG        (*((volatile uint32 *)0x40005510))
#define GPIO_PORTB_PDR_REG        (*((volatile uint32 *)0x40005514))
#define GPIO_PORTB_DEN_REG        (*((volatile uint32 *)0x4000551C))
#define GPIO_PORTB_LOCK_REG       (*((volatile uint32 *)0x40005520))
#define GPIO_PORTB_CR_REG         (*((volatile uint32 *)0x40005524))
#define GPIO_PORTB_AMSEL_REG      (*((volatile uint32 *)0x40005528))
#define GPIO_PORTB_PCTL_REG       (*((volatile uint32 *)0x4000552C))



void Timer2_Count_Init(void){

    SET_BIT(RCGCTIMER,2);    //Enable clock for timer2
    SET_BIT(RCGCGPIO,1);        //enable GPIO clock
    SET_BIT(RCGCGPIO,5);        //enable GPIO clock
    CLEAR_BIT(GPIO_PORTB_DIR_REG,0);        //enable GPIO clock
    CLEAR_BIT(GPIO_PORTF_DIR_REG,4);
    SET_BIT(GPIO_PORTB_DEN_REG,0);        //enable GPIO clock
    SET_BIT(GPIO_PORTF_DEN_REG,4);
    SET_BIT(GPIO_PORTB_AFSEL_REG,0);        //enable GPIO clock
    SET_BIT(GPIO_PORTF_AFSEL_REG,4);
    GPIO_PORTB_PCTL_REG  &=~0x0000000F;
    GPIO_PORTB_PCTL_REG  |=0x00000007;
    GPIO_PORTB_PCTL_REG  &=~0x000F0000;
    GPIO_PORTB_PCTL_REG  |=0x00070000;

    CLEAR_BIT(GPTMCTL2,0);      //Disable timer0 during configuration


    GPTMCFG2=0x04;

    GPTMTAMR2 = 0x17;     //Select one shot timer& Down count selection
//    GPTAMATCHR3 = 0xFFFF;
  //  GPTMTAPMR3 = 0xFF;
    SET_BIT(GPTMCTL2, 2);
    SET_BIT(GPTMCTL2, 3);
    //clear 3
    SET_BIT(GPTMCTL2,0);        //Enable timer3 to start counting
}

uint32 Timer2_Count_Ret(void){

    SET_BIT(GPTMICR2,2);
    while(GET_BIT(GPTMRIS2, 2)==0){}
    uint32 duration1 =  GPTMTAR2;


    SET_BIT(GPTMICR2,2);
    while(GET_BIT(GPTMRIS2, 2)==0){}
    uint32 duration2 =   GPTMTAR2;


    return (duration2 - duration1) &0x00FFFFFF;

}
 */
