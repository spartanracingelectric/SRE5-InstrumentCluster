/*
Put into main.c to initialize interrupts for buttons - change respective PCINTs to respective ATmega328P ports
 
PCMSK1 |= (1<<PCINT8) | (1<<PCINT9) | (1<<PCINT10) | (1<<PCINT11); //Enable pin change interrupts on pc0 -> pc3 pins ( Pin Change Mask Register 1 )
PCICR |= (1<<PCIE1);  //When the PCIE1 bit is set (one) and the I-bit in the status register (SREG) is set (one), pin change interrupt 1 is enabled ( Pin Change Interrupt Control Register )
sei(); //Enables interrupts by setting the global interrupt mask
*/


#ifndef BUTTONS_H_
#define BUTTONS_H_

#define F_CPU 16000000UL

#include <util/delay.h>
#include "1602lcd.h"


/* @WAYNE edit accordingly to whatever the buttons are connected to */
#define BUT1 PINC0
#define BUT2 PINC1
#define BUT3 PINC2
#define BUT4 PINC3

#define PCINT_BUT1 (1<<PCINT8)
#define PCINT_BUT2 (1<<PCINT9)
#define PCINT_BUT3 (1<<PCINT10)
#define PCINT_BUT4 (1<<PCINT11)
/********************************************************************/

#define HOLD_TIME 1000 //Button hold time for bottom buttons on default screen

void buttons_init();

#endif /* BUTTONS_H_ */