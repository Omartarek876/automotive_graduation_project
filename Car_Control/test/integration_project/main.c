

/**
 * main.c
 */

#include "Port_Driver/PORT_Cfg.h"
#include "Int_Ctrl/IntCtrl_Cfg.h"
#include "Port_Driver/PORT.h"
#include "Int_Ctrl/IntCtrl.h"
#include "Gptt_Driver/Gpt.h"
#include "Gptt_Driver/Gpt_cfg.h"
#include "Pwwm_Driver/PWM.h"
#include "Pwwm_Driver/PWM_Cfg.h"

//Test_PWM_Generation_NOISR_Configs(void);

int main(void)
{
    int i =0;
    Port_Init(&Port_Configuration);
    Gpt_Init();
    IntCtrl_Init();
    pwm_init();
    pwm_enable(PWM_NUM_0_1_A);
    while(1)
    {
        Delay_ms(50);
        pwm_AutomaticDutyCycle(PWM_NUM_0_1_A, i);
        i++;
        if (i > 100)
        {
            i = 0;
        }

    }
}
/*
Test_PWM_Generation_NOISR_Configs(void)
{
    int i =0;
    Port_Init(&Port_Configuration);
    Gpt_Init();
    IntCtrl_Init();
    pwm_init();
    pwm_enable(PWM_NUM_0_1_A);
    while(1)
    {
        Delay_ms(50);
        pwm_AutomaticDutyCycle(PWM_NUM_0_1_A, i);
        i++;
        if (i > 100)
        {
            i = 0;
        }

    }
}
*/

