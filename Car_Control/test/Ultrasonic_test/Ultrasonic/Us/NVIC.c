
 /******************************************************************************
* File Name: <NVIC.c>
* Author : Omar Tarek
* Description: Program Source file for the NVIC [Nested Vector Interrupt Controller] Driver.
* Date Created: 12/09/2024
* Micro-Controller: TM4C123GH6PM (TIVA C)
* Micro-Processor: ARM Cortex-M4
 ******************************************************************************/

#include <NVIC.h>
#include <NVIC_cfg.h>
#include "private_registers.h"
#include "Common_Macros.h"


/* Set PRIV Bit(Bit 0) in the CONTROL register to switch to Unprivileged access level */
void Switch_To_Unprivileged(void)
{
    __asm(" MOV R0, #1");      /* Set the Ro register value to 1 */
    __asm(" MSR CONTROL, R0"); /* Set the PRIV Bit(Bit 0) to 1 ... This will switch to Unprivileged access level */
}

void Switch_To_Privileged(void)
{
    __asm(" MOV R0, #0");      /* Set the Ro register value to 1 */
    __asm(" MSR CONTROL, R0"); /* Set the PRIV Bit(Bit 0) to 1 ... This will switch to Unprivileged access level */
}


/*THEY DON'T ENABLE BIT, THEY'RE ALREADY ENABLED, WE JUST ASSIGN PRIORITY TO THEM AND GIVE THEM HANDLER PROTOTYPE IN THE STARTUP CODE*/
void SVC_Init(void)
{
    /* Assign priority level 0 to the SVC Interrupt */
    NVIC_SetInterruptPrioritySw(SVC_INTERRUPT,SVC_INTERRUPT_PRIORITY);
}

/*Just An example*/
void SVC_Handler(void)
{
    unsigned char SVC_Num = 0;
    __asm(" LDR R0, [SP, #24]");  /* Extract the stacked LR register value which vectoring to this handler */
    __asm(" LDRB R0, [R0, #-2]"); /* Extract the SVC number value it is exist in the first byte of address LR-2 */
    __asm(" STR R0, [SP]");       /* Load the R0 value to SVC_Num variable as SP is pointing to SVC_Num location in the stack memory */
    
    switch(SVC_Num)
    {
        case 0:
           /* __asm("MOV R3, #0");       Load the R3 register with value 0 */
           /* __asm("MSR CONTROL, R3");  Clear the PRIV Bit(Bit 0) ... This will switch to Privileged access level */
            Switch_To_Privileged();
            break;
        case 1:
        case 2:
        case 3:  
        case 255:  
            break;
        default:
            break;
    }
}

/*THEY DON'T ENABLE BIT, THEY'RE ALREADY ENABLED, WE JUST ASSIGN PRIORITY TO THEM AND GIVE THEM HANDLER PROTOTYPE IN THE STARTUP CODE*/
void PendSV_Init(void)
{
    /* Assign priority level 7 to the PendSV Interrupt */
    NVIC_SetInterruptPrioritySw(PENDSV_INTERRUPT,PENDSV_INTERRUPT_PRIORITY);
}

void PendSV_Call(void)
{
    /* 2. Pend the PendSV System Exception by set the PENDSV bit in the INTCTRL register 
    We Pend it only, we don't make it, as if there any other interrupt it waits until finished*/
    /*NVIC_SYSTEM_INT_CTRL_REGISTER |= (1<<28);*/
    SET_BIT(NVIC_SYSTEM_INT_CTRL_REGISTER,PENDSV_ENABLE_BIT);    
}

void NVIC_DisableInterrupt(uint8 Interrupt)
{
    /* Disable the interrupt by Setting the interrupt bit in the NVIC Interrupt Disable Register */
    if(Interrupt <= 31)
    {
        SET_BIT(NVIC_DIS0_REG,Interrupt);
    }
    else if((Interrupt >= 32) && (Interrupt <= 63))
    {
        SET_BIT(NVIC_DIS1_REG,(Interrupt-32));
    }
    else if((Interrupt >= 64) && (Interrupt <= 95))
    {
        SET_BIT(NVIC_DIS2_REG,(Interrupt-64));
    }
    else if((Interrupt >= 96) && (Interrupt <= 127))
    {
        SET_BIT(NVIC_DIS3_REG,(Interrupt-96));
    }
    else if((Interrupt >= 128) && (Interrupt <= 138))
    {
        SET_BIT(NVIC_DIS4_REG,(Interrupt-128));
    }
    else
    {
        /*Do Nothing*/
    }
}

void NVIC_EnableInterrupt(uint8 Interrupt)
{
    /* Enable the interrupt by Setting the interrupt bit in the NVIC Interrupt Enable Register */
    if(Interrupt <= 31)
    {
        SET_BIT(NVIC_EN0_REG,Interrupt);
    }
    else if((Interrupt >= 32) && (Interrupt <= 63))
    {
        SET_BIT(NVIC_EN1_REG,(Interrupt-32));
    }
    else if((Interrupt >= 64) && (Interrupt <= 95))
    {
        SET_BIT(NVIC_EN2_REG,(Interrupt-64));
    }
    else if((Interrupt >= 96) && (Interrupt <= 127))
    {
        SET_BIT(NVIC_EN3_REG,(Interrupt-96));
    }
    else if((Interrupt >= 128) && (Interrupt <= 138))
    {
        SET_BIT(NVIC_EN4_REG,(Interrupt-128));
    }
    else
    {
        /*Do Nothing*/
    }
}

void NVIC_SetInterruptPriorityHw(uint8 Interrupt,uint8 priority)
{
    /* Set the interrupt priority by Writing the priority in the priority bits in the NVIC Interrupt Priority Register */
    if((Interrupt <= 138) && (priority <= 7))
   {
        switch(Interrupt)
     {
        case 0:
            NVIC_PRI0_REG = ((NVIC_PRI0_REG & INTA_PRIORITY_BITS_MASK) | (priority << INTA_PRIORITY_START_BIT_POSITION));
            break;    
        case 1:
            NVIC_PRI0_REG = ((NVIC_PRI0_REG & INTB_PRIORITY_BITS_MASK) | (priority << INTB_PRIORITY_START_BIT_POSITION));
            break;
        case 2:
            NVIC_PRI0_REG = ((NVIC_PRI0_REG & INTC_PRIORITY_BITS_MASK) | (priority << INTC_PRIORITY_START_BIT_POSITION));
            break;
        case 3:
            NVIC_PRI0_REG = ((NVIC_PRI0_REG & INTD_PRIORITY_BITS_MASK) | (priority << INTD_PRIORITY_START_BIT_POSITION));
            break;
        case 4:
            NVIC_PRI1_REG = ((NVIC_PRI1_REG & INTA_PRIORITY_BITS_MASK) | (priority << INTA_PRIORITY_START_BIT_POSITION));
            break;    
        case 5:
            NVIC_PRI1_REG = ((NVIC_PRI1_REG & INTB_PRIORITY_BITS_MASK) | (priority << INTB_PRIORITY_START_BIT_POSITION));
            break;
        case 6:
            NVIC_PRI1_REG = ((NVIC_PRI1_REG & INTC_PRIORITY_BITS_MASK) | (priority << INTC_PRIORITY_START_BIT_POSITION));
            break;
        case 7:
            NVIC_PRI1_REG = ((NVIC_PRI1_REG & INTD_PRIORITY_BITS_MASK) | (priority << INTD_PRIORITY_START_BIT_POSITION));
            break;
        case 8:
            NVIC_PRI2_REG = ((NVIC_PRI2_REG & INTA_PRIORITY_BITS_MASK) | (priority << INTA_PRIORITY_START_BIT_POSITION));
            break;    
        case 9:
            NVIC_PRI2_REG = ((NVIC_PRI2_REG & INTB_PRIORITY_BITS_MASK) | (priority << INTB_PRIORITY_START_BIT_POSITION));
            break;
        case 10:
            NVIC_PRI2_REG = ((NVIC_PRI2_REG & INTC_PRIORITY_BITS_MASK) | (priority << INTC_PRIORITY_START_BIT_POSITION));
            break;
        case 11:
            NVIC_PRI2_REG = ((NVIC_PRI2_REG & INTD_PRIORITY_BITS_MASK) | (priority << INTD_PRIORITY_START_BIT_POSITION));
            break;
        case 12:
            NVIC_PRI3_REG = ((NVIC_PRI3_REG & INTA_PRIORITY_BITS_MASK) | (priority << INTA_PRIORITY_START_BIT_POSITION));
            break;    
        case 13:
            NVIC_PRI3_REG = ((NVIC_PRI3_REG & INTB_PRIORITY_BITS_MASK) | (priority << INTB_PRIORITY_START_BIT_POSITION));
            break;
        case 14:
            NVIC_PRI3_REG = ((NVIC_PRI3_REG & INTC_PRIORITY_BITS_MASK) | (priority << INTC_PRIORITY_START_BIT_POSITION));
            break;
        case 15:
            NVIC_PRI3_REG = ((NVIC_PRI3_REG & INTD_PRIORITY_BITS_MASK) | (priority << INTD_PRIORITY_START_BIT_POSITION));
            break;
        case 16:
            NVIC_PRI4_REG = ((NVIC_PRI4_REG & INTA_PRIORITY_BITS_MASK) | (priority << INTA_PRIORITY_START_BIT_POSITION));
            break;    
        case 17:
            NVIC_PRI4_REG = ((NVIC_PRI4_REG & INTB_PRIORITY_BITS_MASK) | (priority << INTB_PRIORITY_START_BIT_POSITION));
            break;
        case 18:
            NVIC_PRI4_REG = ((NVIC_PRI4_REG & INTC_PRIORITY_BITS_MASK) | (priority << INTC_PRIORITY_START_BIT_POSITION));
            break;
        case 19:
            NVIC_PRI4_REG = ((NVIC_PRI4_REG & INTD_PRIORITY_BITS_MASK) | (priority << INTD_PRIORITY_START_BIT_POSITION));
            break;
        case 20:
            NVIC_PRI5_REG = ((NVIC_PRI5_REG & INTA_PRIORITY_BITS_MASK) | (priority << INTA_PRIORITY_START_BIT_POSITION));
            break;    
        case 21:
            NVIC_PRI5_REG = ((NVIC_PRI5_REG & INTB_PRIORITY_BITS_MASK) | (priority << INTB_PRIORITY_START_BIT_POSITION));
            break;
        case 22:
            NVIC_PRI5_REG = ((NVIC_PRI5_REG & INTC_PRIORITY_BITS_MASK) | (priority << INTC_PRIORITY_START_BIT_POSITION));
            break;
        case 23:
            NVIC_PRI5_REG = ((NVIC_PRI5_REG & INTD_PRIORITY_BITS_MASK) | (priority << INTD_PRIORITY_START_BIT_POSITION));
            break;
        case 24:
            NVIC_PRI6_REG = ((NVIC_PRI6_REG & INTA_PRIORITY_BITS_MASK) | (priority << INTA_PRIORITY_START_BIT_POSITION));
            break;    
        case 25:
            NVIC_PRI6_REG = ((NVIC_PRI6_REG & INTB_PRIORITY_BITS_MASK) | (priority << INTB_PRIORITY_START_BIT_POSITION));
            break;
        case 26:
            NVIC_PRI6_REG = ((NVIC_PRI6_REG & INTC_PRIORITY_BITS_MASK) | (priority << INTC_PRIORITY_START_BIT_POSITION));
            break;
        case 27:
            NVIC_PRI6_REG = ((NVIC_PRI6_REG & INTD_PRIORITY_BITS_MASK) | (priority << INTD_PRIORITY_START_BIT_POSITION));
            break;
        case 28:
            NVIC_PRI7_REG = ((NVIC_PRI7_REG & INTA_PRIORITY_BITS_MASK) | (priority << INTA_PRIORITY_START_BIT_POSITION));
            break;    
        case 29:
            NVIC_PRI7_REG = ((NVIC_PRI7_REG & INTB_PRIORITY_BITS_MASK) | (priority << INTB_PRIORITY_START_BIT_POSITION));
            break;
        case 30:
            NVIC_PRI7_REG = ((NVIC_PRI7_REG & INTC_PRIORITY_BITS_MASK) | (priority << INTC_PRIORITY_START_BIT_POSITION));
            break;
        case 31:
            NVIC_PRI7_REG = ((NVIC_PRI7_REG & INTD_PRIORITY_BITS_MASK) | (priority << INTD_PRIORITY_START_BIT_POSITION));
            break;
        case 32:
            NVIC_PRI8_REG = ((NVIC_PRI8_REG & INTA_PRIORITY_BITS_MASK) | (priority << INTA_PRIORITY_START_BIT_POSITION));
            break;    
        case 33:
            NVIC_PRI8_REG = ((NVIC_PRI8_REG & INTB_PRIORITY_BITS_MASK) | (priority << INTB_PRIORITY_START_BIT_POSITION));
            break;
        case 34:
            NVIC_PRI8_REG = ((NVIC_PRI8_REG & INTC_PRIORITY_BITS_MASK) | (priority << INTC_PRIORITY_START_BIT_POSITION));
            break;
        case 35:
            NVIC_PRI8_REG = ((NVIC_PRI8_REG & INTD_PRIORITY_BITS_MASK) | (priority << INTD_PRIORITY_START_BIT_POSITION));
            break;
        case 36:
            NVIC_PRI9_REG = ((NVIC_PRI9_REG & INTA_PRIORITY_BITS_MASK) | (priority << INTA_PRIORITY_START_BIT_POSITION));
            break;    
        case 37:
            NVIC_PRI9_REG = ((NVIC_PRI9_REG & INTB_PRIORITY_BITS_MASK) | (priority << INTB_PRIORITY_START_BIT_POSITION));
            break;
        case 38:
            NVIC_PRI9_REG = ((NVIC_PRI9_REG & INTC_PRIORITY_BITS_MASK) | (priority << INTC_PRIORITY_START_BIT_POSITION));
            break;
        case 39:
            NVIC_PRI9_REG = ((NVIC_PRI9_REG & INTD_PRIORITY_BITS_MASK) | (priority << INTD_PRIORITY_START_BIT_POSITION));
            break;
        case 40:
            NVIC_PRI10_REG = ((NVIC_PRI10_REG & INTA_PRIORITY_BITS_MASK) | (priority << INTA_PRIORITY_START_BIT_POSITION));
            break;    
        case 41:
            NVIC_PRI10_REG = ((NVIC_PRI10_REG & INTB_PRIORITY_BITS_MASK) | (priority << INTB_PRIORITY_START_BIT_POSITION));
            break;
        case 42:
            NVIC_PRI10_REG = ((NVIC_PRI10_REG & INTC_PRIORITY_BITS_MASK) | (priority << INTC_PRIORITY_START_BIT_POSITION));
            break;
        case 43:
            NVIC_PRI10_REG = ((NVIC_PRI10_REG & INTD_PRIORITY_BITS_MASK) | (priority << INTD_PRIORITY_START_BIT_POSITION));
            break;
        case 44:
            NVIC_PRI11_REG = ((NVIC_PRI11_REG & INTA_PRIORITY_BITS_MASK) | (priority << INTA_PRIORITY_START_BIT_POSITION));
            break;    
        case 45:
            NVIC_PRI11_REG = ((NVIC_PRI11_REG & INTB_PRIORITY_BITS_MASK) | (priority << INTB_PRIORITY_START_BIT_POSITION));
            break;
        case 46:
            NVIC_PRI11_REG = ((NVIC_PRI11_REG & INTC_PRIORITY_BITS_MASK) | (priority << INTC_PRIORITY_START_BIT_POSITION));
            break;
        case 47:
            NVIC_PRI11_REG = ((NVIC_PRI11_REG & INTD_PRIORITY_BITS_MASK) | (priority << INTD_PRIORITY_START_BIT_POSITION));
            break;
        case 48:
            NVIC_PRI12_REG = ((NVIC_PRI12_REG & INTA_PRIORITY_BITS_MASK) | (priority << INTA_PRIORITY_START_BIT_POSITION));
            break;    
        case 49:
            NVIC_PRI12_REG = ((NVIC_PRI12_REG & INTB_PRIORITY_BITS_MASK) | (priority << INTB_PRIORITY_START_BIT_POSITION));
            break;
        case 50:
            NVIC_PRI12_REG = ((NVIC_PRI12_REG & INTC_PRIORITY_BITS_MASK) | (priority << INTC_PRIORITY_START_BIT_POSITION));
            break;
        case 51:
            NVIC_PRI12_REG = ((NVIC_PRI12_REG & INTD_PRIORITY_BITS_MASK) | (priority << INTD_PRIORITY_START_BIT_POSITION));
            break;
        case 52:
            NVIC_PRI13_REG = ((NVIC_PRI13_REG & INTA_PRIORITY_BITS_MASK) | (priority << INTA_PRIORITY_START_BIT_POSITION));
            break;    
        case 53:
            NVIC_PRI13_REG = ((NVIC_PRI13_REG & INTB_PRIORITY_BITS_MASK) | (priority << INTB_PRIORITY_START_BIT_POSITION));
            break;
        case 54:
            NVIC_PRI13_REG = ((NVIC_PRI13_REG & INTC_PRIORITY_BITS_MASK) | (priority << INTC_PRIORITY_START_BIT_POSITION));
            break;
        case 55:
            NVIC_PRI13_REG = ((NVIC_PRI13_REG & INTD_PRIORITY_BITS_MASK) | (priority << INTD_PRIORITY_START_BIT_POSITION));
            break;
        case 56:
            NVIC_PRI14_REG = ((NVIC_PRI14_REG & INTA_PRIORITY_BITS_MASK) | (priority << INTA_PRIORITY_START_BIT_POSITION));
            break;    
        case 57:
            NVIC_PRI14_REG = ((NVIC_PRI14_REG & INTB_PRIORITY_BITS_MASK) | (priority << INTB_PRIORITY_START_BIT_POSITION));
            break;
        case 58:
            NVIC_PRI14_REG = ((NVIC_PRI14_REG & INTC_PRIORITY_BITS_MASK) | (priority << INTC_PRIORITY_START_BIT_POSITION));
            break;
        case 59:
            NVIC_PRI14_REG = ((NVIC_PRI14_REG & INTD_PRIORITY_BITS_MASK) | (priority << INTD_PRIORITY_START_BIT_POSITION));
            break;
        case 60:
            NVIC_PRI15_REG = ((NVIC_PRI15_REG & INTA_PRIORITY_BITS_MASK) | (priority << INTA_PRIORITY_START_BIT_POSITION));
            break;    
        case 61:
            NVIC_PRI15_REG = ((NVIC_PRI15_REG & INTB_PRIORITY_BITS_MASK) | (priority << INTB_PRIORITY_START_BIT_POSITION));
            break;
        case 62:
            NVIC_PRI15_REG = ((NVIC_PRI15_REG & INTC_PRIORITY_BITS_MASK) | (priority << INTC_PRIORITY_START_BIT_POSITION));
            break;
        case 63:
            NVIC_PRI15_REG = ((NVIC_PRI15_REG & INTD_PRIORITY_BITS_MASK) | (priority << INTD_PRIORITY_START_BIT_POSITION));
            break;
        case 64:
            NVIC_PRI16_REG = ((NVIC_PRI16_REG & INTA_PRIORITY_BITS_MASK) | (priority << INTA_PRIORITY_START_BIT_POSITION));
            break;    
        case 65:
            NVIC_PRI16_REG = ((NVIC_PRI16_REG & INTB_PRIORITY_BITS_MASK) | (priority << INTB_PRIORITY_START_BIT_POSITION));
            break;
        case 66:
            NVIC_PRI16_REG = ((NVIC_PRI16_REG & INTC_PRIORITY_BITS_MASK) | (priority << INTC_PRIORITY_START_BIT_POSITION));
            break;
        case 67:
            NVIC_PRI16_REG = ((NVIC_PRI16_REG & INTD_PRIORITY_BITS_MASK) | (priority << INTD_PRIORITY_START_BIT_POSITION));
            break;
        case 68:
            NVIC_PRI17_REG = ((NVIC_PRI17_REG & INTA_PRIORITY_BITS_MASK) | (priority << INTA_PRIORITY_START_BIT_POSITION));
            break;    
        case 69:
            NVIC_PRI17_REG = ((NVIC_PRI17_REG & INTB_PRIORITY_BITS_MASK) | (priority << INTB_PRIORITY_START_BIT_POSITION));
            break;
        case 70:
            NVIC_PRI17_REG = ((NVIC_PRI17_REG & INTC_PRIORITY_BITS_MASK) | (priority << INTC_PRIORITY_START_BIT_POSITION));
            break;
        case 71:
            NVIC_PRI17_REG = ((NVIC_PRI17_REG & INTD_PRIORITY_BITS_MASK) | (priority << INTD_PRIORITY_START_BIT_POSITION));
            break;
        case 72:
            NVIC_PRI18_REG = ((NVIC_PRI18_REG & INTA_PRIORITY_BITS_MASK) | (priority << INTA_PRIORITY_START_BIT_POSITION));
            break;    
        case 73:
            NVIC_PRI18_REG = ((NVIC_PRI18_REG & INTB_PRIORITY_BITS_MASK) | (priority << INTB_PRIORITY_START_BIT_POSITION));
            break;
        case 74:
            NVIC_PRI18_REG = ((NVIC_PRI18_REG & INTC_PRIORITY_BITS_MASK) | (priority << INTC_PRIORITY_START_BIT_POSITION));
            break;
        case 75:
            NVIC_PRI18_REG = ((NVIC_PRI18_REG & INTD_PRIORITY_BITS_MASK) | (priority << INTD_PRIORITY_START_BIT_POSITION));
            break;
        case 76:
            NVIC_PRI19_REG = ((NVIC_PRI19_REG & INTA_PRIORITY_BITS_MASK) | (priority << INTA_PRIORITY_START_BIT_POSITION));
            break;    
        case 77:
            NVIC_PRI19_REG = ((NVIC_PRI19_REG & INTB_PRIORITY_BITS_MASK) | (priority << INTB_PRIORITY_START_BIT_POSITION));
            break;
        case 78:
            NVIC_PRI19_REG = ((NVIC_PRI19_REG & INTC_PRIORITY_BITS_MASK) | (priority << INTC_PRIORITY_START_BIT_POSITION));
            break;
        case 79:
            NVIC_PRI19_REG = ((NVIC_PRI19_REG & INTD_PRIORITY_BITS_MASK) | (priority << INTD_PRIORITY_START_BIT_POSITION));
            break;
        case 80:
            NVIC_PRI20_REG = ((NVIC_PRI20_REG & INTA_PRIORITY_BITS_MASK) | (priority << INTA_PRIORITY_START_BIT_POSITION));
            break;    
        case 81:
            NVIC_PRI20_REG = ((NVIC_PRI20_REG & INTB_PRIORITY_BITS_MASK) | (priority << INTB_PRIORITY_START_BIT_POSITION));
            break;
        case 82:
            NVIC_PRI20_REG = ((NVIC_PRI20_REG & INTC_PRIORITY_BITS_MASK) | (priority << INTC_PRIORITY_START_BIT_POSITION));
            break;
        case 83:
            NVIC_PRI20_REG = ((NVIC_PRI20_REG & INTD_PRIORITY_BITS_MASK) | (priority << INTD_PRIORITY_START_BIT_POSITION));
            break;
        case 84:
            NVIC_PRI21_REG = ((NVIC_PRI21_REG & INTA_PRIORITY_BITS_MASK) | (priority << INTA_PRIORITY_START_BIT_POSITION));
            break;    
        case 85:
            NVIC_PRI21_REG = ((NVIC_PRI21_REG & INTB_PRIORITY_BITS_MASK) | (priority << INTB_PRIORITY_START_BIT_POSITION));
            break;
        case 86:
            NVIC_PRI21_REG = ((NVIC_PRI21_REG & INTC_PRIORITY_BITS_MASK) | (priority << INTC_PRIORITY_START_BIT_POSITION));
            break;
        case 87:
            NVIC_PRI21_REG = ((NVIC_PRI21_REG & INTD_PRIORITY_BITS_MASK) | (priority << INTD_PRIORITY_START_BIT_POSITION));
            break;
        case 88:
            NVIC_PRI22_REG = ((NVIC_PRI22_REG & INTA_PRIORITY_BITS_MASK) | (priority << INTA_PRIORITY_START_BIT_POSITION));
            break;    
        case 89:
            NVIC_PRI22_REG = ((NVIC_PRI22_REG & INTB_PRIORITY_BITS_MASK) | (priority << INTB_PRIORITY_START_BIT_POSITION));
            break;
        case 90:
            NVIC_PRI22_REG = ((NVIC_PRI22_REG & INTC_PRIORITY_BITS_MASK) | (priority << INTC_PRIORITY_START_BIT_POSITION));
            break;
        case 91:
            NVIC_PRI22_REG = ((NVIC_PRI22_REG & INTD_PRIORITY_BITS_MASK) | (priority << INTD_PRIORITY_START_BIT_POSITION));
            break;
        case 92:
            NVIC_PRI23_REG = ((NVIC_PRI23_REG & INTA_PRIORITY_BITS_MASK) | (priority << INTA_PRIORITY_START_BIT_POSITION));
            break;    
        case 93:
            NVIC_PRI23_REG = ((NVIC_PRI23_REG & INTB_PRIORITY_BITS_MASK) | (priority << INTB_PRIORITY_START_BIT_POSITION));
            break;
        case 94:
            NVIC_PRI23_REG = ((NVIC_PRI23_REG & INTC_PRIORITY_BITS_MASK) | (priority << INTC_PRIORITY_START_BIT_POSITION));
            break;
        case 95:
            NVIC_PRI23_REG = ((NVIC_PRI23_REG & INTD_PRIORITY_BITS_MASK) | (priority << INTD_PRIORITY_START_BIT_POSITION));
            break;
        case 96:
            NVIC_PRI24_REG = ((NVIC_PRI24_REG & INTA_PRIORITY_BITS_MASK) | (priority << INTA_PRIORITY_START_BIT_POSITION));
            break;    
        case 97:
            NVIC_PRI24_REG = ((NVIC_PRI24_REG & INTB_PRIORITY_BITS_MASK) | (priority << INTB_PRIORITY_START_BIT_POSITION));
            break;
        case 98:
            NVIC_PRI24_REG = ((NVIC_PRI24_REG & INTC_PRIORITY_BITS_MASK) | (priority << INTC_PRIORITY_START_BIT_POSITION));
            break;
        case 99:
            NVIC_PRI24_REG = ((NVIC_PRI24_REG & INTD_PRIORITY_BITS_MASK) | (priority << INTD_PRIORITY_START_BIT_POSITION));
            break;
        case 100:
            NVIC_PRI25_REG = ((NVIC_PRI25_REG & INTA_PRIORITY_BITS_MASK) | (priority << INTA_PRIORITY_START_BIT_POSITION));
            break;    
        case 101:
            NVIC_PRI25_REG = ((NVIC_PRI25_REG & INTB_PRIORITY_BITS_MASK) | (priority << INTB_PRIORITY_START_BIT_POSITION));
            break;
        case 102:
            NVIC_PRI25_REG = ((NVIC_PRI25_REG & INTC_PRIORITY_BITS_MASK) | (priority << INTC_PRIORITY_START_BIT_POSITION));
            break;
        case 103:
            NVIC_PRI25_REG = ((NVIC_PRI25_REG & INTD_PRIORITY_BITS_MASK) | (priority << INTD_PRIORITY_START_BIT_POSITION));
            break;
        case 104:
            NVIC_PRI26_REG = ((NVIC_PRI26_REG & INTA_PRIORITY_BITS_MASK) | (priority << INTA_PRIORITY_START_BIT_POSITION));
            break;    
        case 105:
            NVIC_PRI26_REG = ((NVIC_PRI26_REG & INTB_PRIORITY_BITS_MASK) | (priority << INTB_PRIORITY_START_BIT_POSITION));
            break;
        case 106:
            NVIC_PRI26_REG = ((NVIC_PRI26_REG & INTC_PRIORITY_BITS_MASK) | (priority << INTC_PRIORITY_START_BIT_POSITION));
            break;
        case 107:
            NVIC_PRI26_REG = ((NVIC_PRI26_REG & INTD_PRIORITY_BITS_MASK) | (priority << INTD_PRIORITY_START_BIT_POSITION));
            break;
        case 108:
            NVIC_PRI27_REG = ((NVIC_PRI27_REG & INTA_PRIORITY_BITS_MASK) | (priority << INTA_PRIORITY_START_BIT_POSITION));
            break;    
        case 109:
            NVIC_PRI27_REG = ((NVIC_PRI27_REG & INTB_PRIORITY_BITS_MASK) | (priority << INTB_PRIORITY_START_BIT_POSITION));
            break;
        case 110:
            NVIC_PRI27_REG = ((NVIC_PRI27_REG & INTC_PRIORITY_BITS_MASK) | (priority << INTC_PRIORITY_START_BIT_POSITION));
            break;
        case 111:
            NVIC_PRI27_REG = ((NVIC_PRI27_REG & INTD_PRIORITY_BITS_MASK) | (priority << INTD_PRIORITY_START_BIT_POSITION));
            break;
        case 112:
            NVIC_PRI28_REG = ((NVIC_PRI28_REG & INTA_PRIORITY_BITS_MASK) | (priority << INTA_PRIORITY_START_BIT_POSITION));
            break;    
        case 113:
            NVIC_PRI28_REG = ((NVIC_PRI28_REG & INTB_PRIORITY_BITS_MASK) | (priority << INTB_PRIORITY_START_BIT_POSITION));
            break;
        case 114:
            NVIC_PRI28_REG = ((NVIC_PRI28_REG & INTC_PRIORITY_BITS_MASK) | (priority << INTC_PRIORITY_START_BIT_POSITION));
            break;
        case 115:
            NVIC_PRI28_REG = ((NVIC_PRI28_REG & INTD_PRIORITY_BITS_MASK) | (priority << INTD_PRIORITY_START_BIT_POSITION));
            break;
        case 116:
            NVIC_PRI29_REG = ((NVIC_PRI29_REG & INTA_PRIORITY_BITS_MASK) | (priority << INTA_PRIORITY_START_BIT_POSITION));
            break;    
        case 117:
            NVIC_PRI29_REG = ((NVIC_PRI29_REG & INTB_PRIORITY_BITS_MASK) | (priority << INTB_PRIORITY_START_BIT_POSITION));
            break;
        case 118:
            NVIC_PRI29_REG = ((NVIC_PRI29_REG & INTC_PRIORITY_BITS_MASK) | (priority << INTC_PRIORITY_START_BIT_POSITION));
            break;
        case 119:
            NVIC_PRI29_REG = ((NVIC_PRI29_REG & INTD_PRIORITY_BITS_MASK) | (priority << INTD_PRIORITY_START_BIT_POSITION));
            break;
        case 120:
            NVIC_PRI30_REG = ((NVIC_PRI30_REG & INTA_PRIORITY_BITS_MASK) | (priority << INTA_PRIORITY_START_BIT_POSITION));
            break;    
        case 121:
            NVIC_PRI30_REG = ((NVIC_PRI30_REG & INTB_PRIORITY_BITS_MASK) | (priority << INTB_PRIORITY_START_BIT_POSITION));
            break;
        case 122:
            NVIC_PRI30_REG = ((NVIC_PRI30_REG & INTC_PRIORITY_BITS_MASK) | (priority << INTC_PRIORITY_START_BIT_POSITION));
            break;
        case 123:
            NVIC_PRI30_REG = ((NVIC_PRI30_REG & INTD_PRIORITY_BITS_MASK) | (priority << INTD_PRIORITY_START_BIT_POSITION));
            break;
        case 124:
            NVIC_PRI31_REG = ((NVIC_PRI31_REG & INTA_PRIORITY_BITS_MASK) | (priority << INTA_PRIORITY_START_BIT_POSITION));
            break;    
        case 125:
            NVIC_PRI31_REG = ((NVIC_PRI31_REG & INTB_PRIORITY_BITS_MASK) | (priority << INTB_PRIORITY_START_BIT_POSITION));
            break;
        case 126:
            NVIC_PRI31_REG = ((NVIC_PRI31_REG & INTC_PRIORITY_BITS_MASK) | (priority << INTC_PRIORITY_START_BIT_POSITION));
            break;
        case 127:
            NVIC_PRI31_REG = ((NVIC_PRI31_REG & INTD_PRIORITY_BITS_MASK) | (priority << INTD_PRIORITY_START_BIT_POSITION));
            break;
        case 128:
            NVIC_PRI32_REG = ((NVIC_PRI32_REG & INTA_PRIORITY_BITS_MASK) | (priority << INTA_PRIORITY_START_BIT_POSITION));
            break;    
        case 129:
            NVIC_PRI32_REG = ((NVIC_PRI32_REG & INTB_PRIORITY_BITS_MASK) | (priority << INTB_PRIORITY_START_BIT_POSITION));
            break;
        case 130:
            NVIC_PRI32_REG = ((NVIC_PRI32_REG & INTC_PRIORITY_BITS_MASK) | (priority << INTC_PRIORITY_START_BIT_POSITION));
            break;
        case 131:
            NVIC_PRI32_REG = ((NVIC_PRI32_REG & INTD_PRIORITY_BITS_MASK) | (priority << INTD_PRIORITY_START_BIT_POSITION));
            break;
        case 132:
            NVIC_PRI33_REG = ((NVIC_PRI33_REG & INTA_PRIORITY_BITS_MASK) | (priority << INTA_PRIORITY_START_BIT_POSITION));
            break;    
        case 133:
            NVIC_PRI33_REG = ((NVIC_PRI33_REG & INTB_PRIORITY_BITS_MASK) | (priority << INTB_PRIORITY_START_BIT_POSITION));
            break;
        case 134:
            NVIC_PRI33_REG = ((NVIC_PRI33_REG & INTC_PRIORITY_BITS_MASK) | (priority << INTC_PRIORITY_START_BIT_POSITION));
            break;
        case 135:
            NVIC_PRI33_REG = ((NVIC_PRI33_REG & INTD_PRIORITY_BITS_MASK) | (priority << INTD_PRIORITY_START_BIT_POSITION));
            break;
        case 136:
            NVIC_PRI34_REG = ((NVIC_PRI34_REG & INTA_PRIORITY_BITS_MASK) | (priority << INTA_PRIORITY_START_BIT_POSITION));
            break;    
        case 137:
            NVIC_PRI34_REG = ((NVIC_PRI34_REG & INTB_PRIORITY_BITS_MASK) | (priority << INTB_PRIORITY_START_BIT_POSITION));
            break;
        case 138:
            NVIC_PRI34_REG = ((NVIC_PRI34_REG & INTC_PRIORITY_BITS_MASK) | (priority << INTC_PRIORITY_START_BIT_POSITION));
            break;
        default:
            break;
     }
  }
  else
  {
    /*Do Nothing*/
  }
}


void NVIC_SetInterruptPrioritySw(uint8 Interrupt,uint8 priority)
{
    if((Interrupt <= 6) && (priority <= 7))
    {
        switch(Interrupt)
        {
            case MEMORY_MANAGEMENT_INTERRUPT:
            NVIC_SYSTEM_PRI1_REGISTER = ((NVIC_SYSTEM_PRI1_REGISTER & MEMORY_FAULT_PRIORITY_MASK) | (priority << MEMORY_FAULT_PRIORITY_START_BIT_POSITION));
            break;
            case BUS_FAULT_INTERRUPT:
            NVIC_SYSTEM_PRI1_REGISTER = ((NVIC_SYSTEM_PRI1_REGISTER & BUS_FAULT_PRIORITY_MASK) | (priority << BUS_FAULT_PRIORITY_START_BIT_POSITION));
            break;
            case USAGE_FAULT_INTERRUPT:
            NVIC_SYSTEM_PRI1_REGISTER = ((NVIC_SYSTEM_PRI1_REGISTER & USAGE_FAULT_PRIORITY_MASK) | (priority << USAGE_FAULT_PRIORITY_START_BIT_POSITION));
            break;
            case SVC_INTERRUPT:
            NVIC_SYSTEM_PRI2_REGISTER = ((NVIC_SYSTEM_PRI2_REGISTER & SVC_PRIORITY_MASK) | (priority << SVC_PRIORITY_START_BIT_POSITION));
            break;
            case DEBUG_MONITOR_INTERRUPT:
            NVIC_SYSTEM_PRI3_REGISTER = ((NVIC_SYSTEM_PRI3_REGISTER & DEBUG_PRIORITY_MASK) | (priority << DEBUG_PRIORITY_START_BIT_POSITION));
            break;
            case PENDSV_INTERRUPT:
            NVIC_SYSTEM_PRI3_REGISTER = ((NVIC_SYSTEM_PRI3_REGISTER & PENDSV_PRIORITY_MASK) | (priority << PENDSV_PRIORITY_START_BIT_POSITION));
            break;
            case SYSTICK_INTERRUPT:
            NVIC_SYSTEM_PRI3_REGISTER = ((NVIC_SYSTEM_PRI3_REGISTER & SYSTICK_PRIORITY_MASK) | (priority << SYSTICK_PRIORITY_START_BIT_POSITION));
            break;
            default:
            break;
        }
    }
    else
    {
        /*Do Nothing*/
    }
}


/*ENABLE TRIGGERING A FAULT WHEN there's a bus fault*/
void BUS_FAULT_ENABLE(void)
{
    /*Enable Bus Fault Interrupts*/
    NVIC_SetInterruptPrioritySw(BUS_FAULT_INTERRUPT,BUS_FAULT_INTERRUPT_PRIORITY);
    NVIC_SYSTEM_HANDLER_CONTROL_REGISTER = ((NVIC_SYSTEM_HANDLER_CONTROL_REGISTER & BUS_FAULT_ENABLE_MASK) | (1 << BUS_FAULT_ENABLE_BIT));
}

void USAGE_FAULT_ENABLE(void)
{
    /*Enable Usage Fault Interrupts*/
    NVIC_SetInterruptPrioritySw(USAGE_FAULT_INTERRUPT,USAGE_FAULT_INTERRUPT_PRIORITY);
    NVIC_SYSTEM_HANDLER_CONTROL_REGISTER = ((NVIC_SYSTEM_HANDLER_CONTROL_REGISTER & USAGE_FAULT_ENABLE_MASK) | (1 << USAGE_FAULT_ENABLE_BIT));
}

void MEMORY_FAULT_ENABLE(void)
{
    /*Enable Memory Fault Interrupts*/
    NVIC_SetInterruptPrioritySw(MEMORY_MANAGEMENT_INTERRUPT, MEMORY_FAULT_INTERRUPT_PRIORITY);
    NVIC_SYSTEM_HANDLER_CONTROL_REGISTER = ((NVIC_SYSTEM_HANDLER_CONTROL_REGISTER & MEMORY_MANAGEMENT_FAULT_ENABLE_MASK) | (1 << MEMORY_MANAGEMENT_FAULT_ENABLE_BIT));
}

void DIV0_faultEnable(void)
{
    /*Enable Divide by Zero Fault Interrupts*/
    NVIC_SYSTEM_CFG_CTRL_REGISTER |= (1 << 4); 
}
