/******************************************************************************
* File Name: Ultrasonic.c
* Author: ChatGPT
* Description: Ultrasonic Sensor Driver for HC-SR04 using Interrupts
* Date Created: 14/02/2025
* Micro-Controller: TM4C123GH6PM (TIVA C)
******************************************************************************/

#include "Dio.h"
#include "Port.h"
#include "NVIC.h"
#include "SYSTICK.h"
#include "private_registers.h"

//#include "Ultrasonic.h"

// lsa hytghyro
#define TRIGGER_PORT   PORTB_ID
#define TRIGGER_PIN    PIN0_ID
#define ECHO_PORT      PORTB_ID
#define ECHO_PIN       PIN1_ID

static volatile uint32 echo_start_time = 0;
static volatile uint32 echo_end_time = 0;
static volatile uint8 measurement_done = 0;

void Ultrasonic_Init(void) {
    /* Configure Trigger Pin as Output */
    Port_SetPinDirection(TRIGGER_PIN, PORT_OUTPUT_PIN);
    Dio_WriteChannel(TRIGGER_PIN, STD_LOW);

    /* Configure Echo Pin as Input */
    Port_SetPinDirection(ECHO_PIN, PORT_INPUT_PIN);

    /* Enable Interrupt on Echo Pin */
    NVIC_EnableInterrupt(GPIO_PORTB_INTERRUPT_NUMBER); //hghyr 3ala hasb hya anhi port
}

void Ultrasonic_Trigger(void) {
    /* Send a 10us pulse on Trigger Pin */
    Dio_WriteChannel(TRIGGER_PIN, STD_HIGH);
    Systick_Delay(10);  // Use available SysTick delay function
    Dio_WriteChannel(TRIGGER_PIN, STD_LOW);
}

void GPIOB_Handler(void) {
    if (Dio_ReadChannel(ECHO_PIN) == STD_HIGH) {
        /* Rising Edge: Start Time */
        echo_start_time = SYSTICK_RELOAD_REG - SYSTICK_CURRENT_REG; // Use SysTick Current Value
    } else {
        /* Falling Edge: End Time */
        echo_end_time = SYSTICK_RELOAD_REG - SYSTICK_CURRENT_REG;
        measurement_done = 1;
    }
    NVIC_DisableInterrupt(GPIO_PORTB_INTERRUPT_NUMBER); // Use appropriate NVIC clear function
}

float Ultrasonic_GetDistance(void) {
    if (!measurement_done) {
        return -1.0; // No valid measurement yet
    }

    uint32 time_elapsed = echo_start_time - echo_end_time;
    float distance = (time_elapsed * 0.0343) / 2; // Convert to cm
    measurement_done = 0;
    return distanceee;
}
x
