#include "PWM_Driver/PWM.h"
#include "Port_Driver/Port.h"
#include "Dio_Driver/Dio.h"
#include <stdint.h>
#include "PWM_Driver/tm4c123gh6pm.h"


// A simple delay routine (adjust loop count for your system clock)
void delay_ms(uint32_t ms)
{
    volatile uint32_t i;
    while (ms--)
    {
        for (i = 0; i < 16000; i++) {} // Roughly 1 ms delay at 16MHz
    }
}


int main(void) {

    // Initialize ports and DIO
    Port_Init(&Port_Configuration);
    Dio_Init(&Dio_Configuration);
    // (Optional) Update system clock settings if needed.
       SYSCTL_RCC_R |= (1 << 22);

       // Initialize the PWM driver (configures three channels)
       PWM_Init();

       // Variables for the breathing effect:
       // We want the full cycle (0 to 100 and back to 0) to take 2000ms.
       // Using a step of 2 means there are 50 steps to go from 0 to 100.
       // With a 20 ms delay per step, the increase takes 50 x 20 = 1000 ms,
       // and the decrease takes another 1000 ms.
       uint8_t duty = 0;
       int8_t step = 2; // 2% change per update

       while (1)
       {
           duty += step;
           if (duty >= 100)
           {
               duty = 100;
               step = -2;
           }
           else if (duty == 0)
           {
               step = 2;
           }

           // Update the duty cycle for all three PWM channels
           PWM_SetDuty(PWM_CHANNEL_0, duty);
           PWM_SetDuty(PWM_CHANNEL_1, duty);
           PWM_SetDuty(PWM_CHANNEL_2, duty);

           delay_ms(2); // 20 ms delay per update gives a full cycle in 2000 ms
       }

}

