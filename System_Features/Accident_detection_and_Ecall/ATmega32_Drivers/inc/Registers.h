/*
 * Registers.h
 *
 *  Created on: Sep 15, 2023
 *      Author: omar tarek
 */

#ifndef INC_REGISTERS_H_
#define INC_REGISTERS_H_

#include "StdTypes.h"

/************* PORTA Registers *************/
#define DDRA		*((volatile u8*) 0x3A) // 00111010
#define PORTA		*((volatile u8*) 0x3B)
#define PINA		*((const volatile u8*) 0x39) 

/************* PORTB Registers *************/
#define DDRB		*((volatile u8*) 0x37)
#define PORTB		*((volatile u8*) 0x38)
#define PINB		*((const volatile u8*) 0x36)

/************* PORTC Registers *************/
#define DDRC		*((volatile u8*) 0x34)
#define PORTC		*((volatile u8*) 0x35)
#define PINC		*((const volatile u8*) 0x33)

/************* PORTD Registers *************/
#define DDRD		*((volatile u8*) 0x31)
#define PORTD		*((volatile u8*) 0x32)
#define PIND		*((const volatile u8*) 0x30)

/************* GIE Registers *************/
#define SREG		*((volatile u8*) 0x5F)

/************* ExtInt Registers *************/
#define MCUCR		*((volatile u8*) 0x55)
#define MCUCSR		*((volatile u8*) 0x54)
#define GICR		*((volatile u8*) 0x5B)
#define GIFR		*((volatile u8*) 0x5A)

/************* ADC Registers *************/
#define ADMUX		*((volatile u8*) 0x27)
#define ADCSRA		*((volatile u8*) 0x26)
#define ADCDATA		*((volatile u16*) 0x24)
#define ADCL		*((volatile u8*) 0x24)
#define ADCH		*((volatile u8*) 0x25)
#define SFIOR		*((volatile u8*) 0x50)

/************* Timer Registers *************/
#define TCCR0		*((volatile u8*) 0x53)
#define TCNT0		*((volatile u8*) 0x52)
#define OCR0		*((volatile u8*) 0x5C)

#define TCCR1A		*((volatile u8*) 0x4F)
#define TCCR1B		*((volatile u8*) 0x4E)
#define TCNT1L		*((volatile u8*) 0x4C)
#define TCNT1H		*((volatile u8*) 0x4D)
#define TCNT1		*((volatile u16*) 0x4C)
#define OCR1AL		*((volatile u8*) 0x4A)
#define OCR1AH		*((volatile u8*) 0x4B)
#define OCR1A		*((volatile u16*) 0x4A)
#define OCR1BL		*((volatile u8*) 0x48)
#define OCR1BH		*((volatile u8*) 0x49)
#define OCR1B		*((volatile u16*) 0x48)
#define ICR1L		*((volatile u8*) 0x46)
#define ICR1H		*((volatile u8*) 0x47)
#define ICR1		*((volatile u16*) 0x46)

#define TCCR2		*((volatile u8*) 0x45)
#define TCNT2		*((volatile u8*) 0x44)
#define OCR2		*((volatile u8*) 0x43)

#define TIMSK		*((volatile u8*) 0x59)
#define TIFR		*((volatile u8*) 0x58)

/************* Watchdog Timer Registers *************/
#define WDTCR		*((volatile u8*) 0x41)

/************* Uart Registers *************/
#define UDR         *((volatile u8*) 0x2C)
#define UCSRA		*((volatile u8*) 0x2B)
#define UCSRB		*((volatile u8*) 0x2A)
#define UCSRC		*((volatile u8*) 0x40)
#define UBRRL		*((volatile u8*) 0x29)
#define UBRRH		*((volatile u8*) 0x40)

/************* SPI Registers *************/
#define SPCR        *((volatile u8*) 0x2D)
#define SPSR		*((volatile u8*) 0x2E)
#define SPDR		*((volatile u8*) 0x2F)

/************* I2C Registers *************/
#define TWBR        *((volatile u8*) 0x20)
#define TWCR		*((volatile u8*) 0x56)
#define TWSR		*((volatile u8*) 0x21)
#define TWDR		*((volatile u8*) 0x23)
#define TWAR		*((volatile u8*) 0x22)

#endif /* INC_REGISTERS_H_ */
