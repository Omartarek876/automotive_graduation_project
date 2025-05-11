/*
 * control_app.c
 *
 *  Created on: Nov 21, 2024
 *      Author: Omar Tarek
 */

#ifndef CONTROL_DRIVER_CONTROL_APP_C_
#define CONTROL_DRIVER_CONTROL_APP_C_



#include "control_app.h"


void car_init (void){
    Port_Init(&Port_Configuration);
    Dio_Init(&Dio_Configuration);

  //  uartInit(&uart0_cfg);
    uartInit(&uart1_cfg);

    /* init the four pins of control motion forword and backword*/
    Dio_WriteChannel(DIoconf_ctrl1_CHANNEL_ID_INDEX , STD_LOW);
    Dio_WriteChannel(DIoconf_ctrl2_CHANNEL_ID_INDEX , STD_LOW);
    Dio_WriteChannel(DIoconf_ctrl3_CHANNEL_ID_INDEX , STD_LOW);
    Dio_WriteChannel(DIoconf_ctrl4_CHANNEL_ID_INDEX , STD_LOW);

    /* init the two pins of control motion right and left*/
    Dio_WriteChannel(DIoconf_ctrl5_CHANNEL_ID_INDEX , STD_LOW);
    Dio_WriteChannel(DIoconf_ctrl6_CHANNEL_ID_INDEX , STD_LOW);
}


void car_forword (void){
    Dio_WriteChannel(DIoconf_ctrl1_CHANNEL_ID_INDEX , STD_HIGH);
    Dio_WriteChannel(DIoconf_ctrl2_CHANNEL_ID_INDEX , STD_LOW);
    Dio_WriteChannel(DIoconf_ctrl3_CHANNEL_ID_INDEX , STD_HIGH);
    Dio_WriteChannel(DIoconf_ctrl4_CHANNEL_ID_INDEX , STD_LOW);
}
void car_backword (void){
    Dio_WriteChannel(DIoconf_ctrl1_CHANNEL_ID_INDEX , STD_LOW);
    Dio_WriteChannel(DIoconf_ctrl2_CHANNEL_ID_INDEX , STD_HIGH);
    Dio_WriteChannel(DIoconf_ctrl3_CHANNEL_ID_INDEX , STD_LOW);
    Dio_WriteChannel(DIoconf_ctrl4_CHANNEL_ID_INDEX , STD_HIGH);

    Dio_WriteChannel(DIoconf_ctrl6_CHANNEL_ID_INDEX , STD_LOW);
    Dio_WriteChannel(DIoconf_ctrl5_CHANNEL_ID_INDEX , STD_LOW);
}
void car_right (void){
    Dio_WriteChannel(DIoconf_ctrl5_CHANNEL_ID_INDEX , STD_HIGH);
    Dio_WriteChannel(DIoconf_ctrl6_CHANNEL_ID_INDEX , STD_LOW);
}
void car_left (void){
    Dio_WriteChannel(DIoconf_ctrl6_CHANNEL_ID_INDEX , STD_HIGH);
    Dio_WriteChannel(DIoconf_ctrl5_CHANNEL_ID_INDEX , STD_LOW);
}

void car_stop (void){
    /* init the four pins of control motion forword and backword*/
    Dio_WriteChannel(DIoconf_ctrl1_CHANNEL_ID_INDEX , STD_LOW);
    Dio_WriteChannel(DIoconf_ctrl2_CHANNEL_ID_INDEX , STD_LOW);
    Dio_WriteChannel(DIoconf_ctrl3_CHANNEL_ID_INDEX , STD_LOW);
    Dio_WriteChannel(DIoconf_ctrl4_CHANNEL_ID_INDEX , STD_LOW);

    /* init the two pins of control motion right and left*/
    Dio_WriteChannel(DIoconf_ctrl5_CHANNEL_ID_INDEX , STD_LOW);
    Dio_WriteChannel(DIoconf_ctrl6_CHANNEL_ID_INDEX , STD_LOW);
}

void Delay_MS(unsigned long long n)
{
    volatile unsigned long long count = 0;
    while(count++ < (NUMBER_OF_ITERATIONS_PER_ONE_MILI_SECOND * n) );
}

#endif /* CONTROL_DRIVER_CONTROL_APP_C_ */
