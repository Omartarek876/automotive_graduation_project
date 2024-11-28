#include <PWM_Channel_10kHz/PWM_Driver.h>

volatile uint32 PWM_Load_Value;
volatile uint32* PWM_CTL_R[]={&PWM0_0_CTL_R,&PWM0_1_CTL_R,&PWM0_2_CTL_R,&PWM0_3_CTL_R,                   //Array of CTL Registers for each module
                              &PWM1_0_CTL_R,&PWM1_1_CTL_R,&PWM1_2_CTL_R,&PWM1_3_CTL_R};

volatile uint32* PWM_GENA_R[]={&PWM0_0_GENA_R,&PWM0_1_GENA_R,&PWM0_2_GENA_R,&PWM0_3_GENA_R,              //Array of GENA Registers for each module
                                 &PWM1_0_GENA_R,&PWM1_1_GENA_R,&PWM1_2_GENA_R,&PWM1_3_GENA_R};

volatile uint32* PWM_GENB_R[]={&PWM0_0_GENB_R,&PWM0_1_GENB_R,&PWM0_2_GENB_R,&PWM0_3_GENB_R,              //Array of GENB Registers for each module
                                 &PWM1_0_GENB_R,&PWM1_1_GENB_R,&PWM1_2_GENB_R,&PWM1_3_GENB_R};

volatile uint32* PWM_Load_R[]={&PWM0_0_LOAD_R,&PWM0_1_LOAD_R,&PWM0_2_LOAD_R,&PWM0_3_LOAD_R,              //Array of LOAD Registers for each module
                                 &PWM1_0_LOAD_R,&PWM1_1_LOAD_R,&PWM1_2_LOAD_R,&PWM1_3_LOAD_R};

volatile uint32* PWM_CMPB_R[]={&PWM0_0_CMPB_R,&PWM0_1_CMPB_R,&PWM0_2_CMPB_R,&PWM0_3_CMPB_R,              //Array of CMPB Registers for each module
                                 &PWM1_0_CMPB_R,&PWM1_1_CMPB_R,&PWM1_2_CMPB_R,&PWM1_3_CMPB_R};

volatile uint32* PWM_CMPA_R[]={&PWM0_0_CMPA_R,&PWM0_1_CMPA_R,&PWM0_2_CMPA_R,&PWM0_3_CMPA_R,              //Array of CMPA Registers for each module
                                 &PWM1_0_CMPA_R,&PWM1_1_CMPA_R,&PWM1_2_CMPA_R,&PWM1_3_CMPA_R};


void CLK_PWM_Enable(uint32 PWM_module)                                                                        //To Eable CLK for specific port
{
  SYSCTL_RCGCPWM_REG |= 1UL << PWM_module;
  volatile uint32 dummy = SYSCTL_RCGCPWM_REG ;
}

void Use_PWM_Divisor(uint32 System_Clock,uint32 PWM_Clock)                                                       //To set PWM Clock
{
  SYSCTL_RCC_REG |= 0x100000;                                                                                //Use PWM divider
  SYSCTL_RCC_REG &= ~(0xE0000);                                                                              //Clear Divison fields
  uint32 division = System_Clock/PWM_Clock;                                                              //calculate division factor
  uint32 divisor = (uint32)(log(division)/log(2))-1;                                                        //calculate reminder factor
  SYSCTL_RCC_REG |= divisor << 17;                                                                           //Feed reminder factor
}

void PWM_Generator_Enable(uint32 PWM_module,uint32 Generator,char type[10])                                      //Enable or Disable specific Generator
{
  int index = Generator +4*PWM_module;                                                                     //To calculate the Index
  if(strcmp(type , "Enable")==0)
  {
    *PWM_CTL_R[index] |= 1UL << 0;                                                                         //To Enable Generator
  }
  else if (strcmp(type , "Disable")==0)
  {
    *PWM_CTL_R[index] &= ~(1UL << 0);                                                                      //To Disable Generator
  }
}

void PWM_Action_For_Load(uint32 PWM_module,uint32 Generator,char Comparator,char type[10])                       //Action to be taken when the counter matches the value in the load register
{
  int index = Generator +4*PWM_module;                                                                     //To calculate the Index
  if(Comparator == 'A')
  {
    if(strcmp(type , "HIGH")==0)
    {
      *PWM_GENA_R[index] |= 0x3 << 2;                                                                      //Drive PWMA High
    }
    else if(strcmp(type , "LOW")==0)
    {
      *PWM_GENA_R[index] |= 0x2 << 2;                                                                      //Drive PWMA Low
    }
  }
  else if(Comparator == 'B')
  {
      if(strcmp(type , "HIGH")==0)
    {
      *PWM_GENB_R[index] |= 0x3 << 2;                                                                      //Drive PWMB High
    }
    else if(strcmp(type , "LOW")==0)
    {
      *PWM_GENB_R[index] |= 0x2 << 2;                                                                      //Drive PWMB High
    }
  }
}
void PWM_Action_Comparator_Down(uint32 PWM_module,uint32 Generator,char Comparator,char type[10])               //Action to be taken when the counter matches the value in the Comparator
{
  int index = Generator +4*PWM_module;                                                                    //To calculate the Index
  if(Comparator == 'A')
  {
    if(strcmp(type , "HIGH")==0)
    {
      *PWM_GENA_R[index] |= 0x3 << 6;                                                                     //Drive PWMA High
    }
    else if(strcmp(type , "LOW")==0)
    {
      *PWM_GENA_R[index] |= 0x2 << 6;                                                                     //Drive PWMA Low
    }
  }
  else if(Comparator == 'B')
  {
      if(strcmp(type , "HIGH")==0)
    {
      *PWM_GENB_R[index] |= 0x3 << 10;                                                                   //Drive PWMB High
    }
    else if(strcmp(type , "LOW")==0)
    {
      *PWM_GENB_R[index] |= 0x2 << 10;                                                                   //Drive PWMB Low
    }
  }
}

void PWM_Set_Frequency(uint32 PWM_module,uint32 Generator,uint32 Frequency,uint32 PWM_Clock_MHz)                     //Set the Frequency for the pwm
{
    uint32 period = PWM_Clock_MHz*1000000/Frequency;
    uint32 bits = (uint32)(log(period)/log(2));
 if (bits >16)
 {
    //Debug_Led("Red",1);                                                                                 //To turn on the red led if you exceeded the register bits
    printf("Load Bits is Greater than 16");
 }
 PWM_Load_Value = period;
 uint32 index = Generator +4*PWM_module;                                                                   //To calculate the Index
 *PWM_Load_R[index] = period - 1;                                                                       //Load Frequency
}

void PWM_Set_Duty_Cycle(uint32 PWM_module,uint32 Generator,char Comparator,uint32 Duty)                          //To determine the duty cycle
{
    uint32 duty = Duty*PWM_Load_Value/100;
    uint32 bits = (uint32)(log(duty)/log(2));
 if (bits >16)
 {
    //Debug_Led("RED",1);                                                                                 //To turn on the red led if you exceeded the register bits
    printf("Load Bits is Greater than 16");
 }
 uint32 index = Generator +4*PWM_module;                                                                  //To calculate the Index
  if(Comparator == 'A')
  {
    *PWM_CMPA_R[index] = duty-1;                                                                        //Load duty for Comparator A
  }
  else if(Comparator == 'B')
  {
    *PWM_CMPB_R[index] = duty-1;                                                                        //Load duty for Comparator B
  }
}

void PWM_Module_Start(uint32 PWM_module)                                                                   //To start the pwm Module
{
  PWM0_ENABLE_R |= 0x00000002;
}
