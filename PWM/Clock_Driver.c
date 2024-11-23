#include "Clock_Driver.h"

void Set_Clock_MHz(uint32 Crystal_Frequency_Mhz, uint32 System_Clock)
{
  SYSCTL_RCC2_REG |= 0x80000000;                                                  //To Use RCC2
  SYSCTL_RCC2_REG |= 0x800;                                                       //The system clock is derived from the OSC source and divided by the divisor specified by SYSDIV2.
  SYSCTL_RCC_REG &= ~(0x7C0);                                                     //Clear crystal value attached to the main oscillator
  switch (Crystal_Frequency_Mhz) {
            case 5:
                SYSCTL_RCC_REG |= CLK_5Mhz << 6;
                break;
            case 6:
                SYSCTL_RCC_REG |= CLK_6Mhz << 6;
                break;
            case 8:
                SYSCTL_RCC_REG |= CLK_8Mhz << 6;
                break;
            case 10:
                SYSCTL_RCC_REG |= CLK_10Mhz << 6;
                break;
            case 12:
                SYSCTL_RCC_REG |= CLK_12Mhz << 6;
                break;
            case 16:
                SYSCTL_RCC_REG |= CLK_16Mhz << 6;
                break;
            case 18:
                SYSCTL_RCC_REG |= CLK_18Mhz << 6;
                break;
            case 20:
                SYSCTL_RCC_REG |= CLK_20Mhz << 6;
                break;
            case 24:
                SYSCTL_RCC_REG |= CLK_24Mhz << 6;
                break;
            case 25:
                SYSCTL_RCC_REG |= CLK_25Mhz << 6;
                break;
            default:
                break;
        }
  SYSCTL_RCC2_REG &= ~(0x70);                                                     //Configure for main oscillator source
  SYSCTL_RCC2_REG &= ~(0x2000);                                                   //Activate PLL by clearing PWRDN
  SYSCTL_RCC2_REG |= 0x40000000;                                                  //Append the SYSDIV2LSB bit to the SYSDIV2 field to create a 7 bit divisor using the 400 MHz PLL output
  uint32_t divisor = 400/System_Clock;                                          //calculate Divisor value
  SYSCTL_RCC2_REG &= ~(0x1FC00000);                                               //Clear Clk register
  SYSCTL_RCC2_REG |= ((divisor)-1) << 22;                                         //Load Divisor value
  while((SYSCTL_RIS_REG & SYSCTL_RIS_PLLLRIS)==0){};                                //wait untill take effect
  SYSCTL_RCC2_REG &= ~(0x800);
}
