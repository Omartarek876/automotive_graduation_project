#include "control_driver/control_app.h"

uint8 Motion_Flag;
uint8 Start_Control_Flag = 0;
uint8 Motion_Control_Char;


int main(void) {
    car_init();

    while (1)
    {
        UART_sendByte(4 , 0x03);
    Delay_MS(50);
    }
}
