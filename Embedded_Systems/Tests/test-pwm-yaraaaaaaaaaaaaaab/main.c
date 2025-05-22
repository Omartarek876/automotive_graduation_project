#include "pwm/PWMSoftware.h"
#include "control_driver/control_app.h"
#include <stdint.h>

// Two separate duty cycle variables and pins
extern void DelayMs(uint32_t ms);

int main(void){

    uint8_t duty = 0;
    uint8 dataReceived;
    uint8 Motion_Control_Char = 0;

    PWMSoftware_Init(PA7, PWM_1KHZ_SYSCLCK_16MHz);
    PWMSoftware_SetDuty(20);
    DelayMs(5);

    car_init();

    while(1){
        // Read and control car
        dataReceived = uart_RecieveByte(1);
        Motion_Control_Char = dataReceived - 48;

        switch (Motion_Control_Char) {
            case 1 :
                car_forword();
                UART_SendString(1 , "ack1");
                break;
            case 2 :
                car_backword();
                UART_SendString(1 , "ACK2");
                break;
            case 3 :
                car_right();
                UART_SendString(1 , "ACK3");
                break;
            case 4 :
                car_left();
                UART_SendString(1 , "ACK4");
                break;
            case 5 :
                car_stop();
                UART_SendString(1 , "ACK5");
                break;
            default :
                UART_SendString(1 , "N-ACK");
                break;
        }

    }
}

void DelayMs(uint32_t ms) {
    volatile uint32_t time;
    while (ms--) {
        time = 72724 * 2 / 91;  // ~1ms at 80MHz
        while (time--) {}
    }
}
