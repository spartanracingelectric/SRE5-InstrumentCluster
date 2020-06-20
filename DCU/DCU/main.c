#ifndef F_CPU
#define F_CPU 16000000UL
#endif

<<<<<<< HEAD
=======
#include <avr/io.h>

>>>>>>> b9cac8ece73d55e93f112cdffe9fb454686442e4
///////////////LED Section///////////////
//Indicator LEDs
#define RGB_Addr //LED Indicators Device Address
#define RGB1 1 //Indicator LED 1
#define RGB2 2 //Indicator LED 2
<<<<<<< HEAD
///////////////LED Section///////////////

#include <avr/io.h>	
=======
///////////////LED Section END///////////////


>>>>>>> b9cac8ece73d55e93f112cdffe9fb454686442e4
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/twi.h>
#include "i2c.h"
#include "led.h"
<<<<<<< HEAD

enum state{state_wake, state_main, state_settings, state_debug,};
=======
#include "buttons.h"


enum state{state_wake, state_main, state_settings, state_debug,};
	
	
>>>>>>> b9cac8ece73d55e93f112cdffe9fb454686442e4
int main(void) {

	int state = state_wake; //every time the car or instrument cluster turns on, start at STATE_WAKE

	while (1) 
	{
		switch (state)
		{
<<<<<<< HEAD
			//STATE_WAKE
			case 1: //stateNum == 1, wake
				/*
				- Display LCD message to test if LCD works
				- LED patterns and blinks to test they are working too
				- Basically a bunch of statements to test hardware, preferably quick ones to allow for functionality ASAP
				*/
				twi_init();
				timer_Init(); 
=======
			///////////////STATE_WAKE///////////////
			case state_wake: 
				//INITIALIZE BUTTON INTERRUPTS
				PCMSK1 |= (1<<PCINT8) | (1<<PCINT9) | (1<<PCINT10) | (1<<PCINT11); //Enable pin change interrupts on pc0 -> pc3 pins ( Pin Change Mask Register 1 )
				PCICR |= (1<<PCIE1);  //When the PCIE1 bit is set (one) and the I-bit in the status register (SREG) is set (one), pin change interrupt 1 is enabled ( Pin Change Interrupt Control Register )
				sei(); //Enables interrupts by setting the global interrupt mask
								
				//INITIALIZE I2C and PWM Timer
				twi_init();
				timer_Init(); 
				
>>>>>>> b9cac8ece73d55e93f112cdffe9fb454686442e4
				//Blink RPM INDICATOR
				rpm_write(0x0); 
				_delay_ms(300);
				rpm_write(0xFFFF);
				_delay_ms(300);
<<<<<<< HEAD
				//BLINK INDICATOR LEDS
				for (int i =0; i<2; i++)
				{
					rgb_set(i, BLACK);
					_delay_ms(300);
					rgb_set(i, WHITE);
					_delay_ms(300);
					rgb_set(i, BLACK);
				}
				state = state_main;
				break;
				
				
				//STATE_MAIN
			case 2: //stateNum == 2, main screen
=======
				
				//BLINK INDICATOR LEDS
				for (int i =0; i<2; i++)
				{
					rgb_set(i, BLUE);
					_delay_ms(300);
					rgb_set(i, RED);
					_delay_ms(300);
					rgb_set(i, BLUE);
				}
				
				state = state_main;
				break;
			///////////////STATE_WAKE_END///////////////
				
				
			///////////////STATE_MAIN///////////////
			case state_main: 
>>>>>>> b9cac8ece73d55e93f112cdffe9fb454686442e4
				/*
				- Take in and process CAN packets
				- Record CAN Packet to SD Card
				- Update LCD/LEDs
				- Read for button press to change engine-state
				*/
<<<<<<< HEAD
				state = state_settings;
				break;
			
			
			/**
			//STATE_SETTINGS
			case 3: //stateNum == 3, settings menu
=======
				
				button_function_0();
				button_function_1();
				button_function_2();
				button_function_3();
				state = state_settings;
				break;
			///////////////STATE_MAIN_EMD///////////////
			
			
			///////////////STATE_SETTINGS///////////////
			case state_settings: //stateNum == 3, settings menu
>>>>>>> b9cac8ece73d55e93f112cdffe9fb454686442e4
				/*
				- LCD controlled, change settings with buttons (if (buttonPressed) to toggle through and select in a menu)
				- Timeout in case driver accidentally hits button, maybe like 15 seconds -> stateNum = 2 to go back to STATE_MAIN
				- On settings exit, stateNum = 2 to go back to STATE_MAIN
				- break;
				*/
				state = state_debug;
				break;
<<<<<<< HEAD
			
			
			//STATE_DEBUG
			case 4:  //stateNum == 4, debug menu
=======
			///////////////STATE_SETTINGS_END///////////////
			
			///////////////STATE_DEBUG///////////////
			case state_debug:  //stateNum == 4, debug menu
>>>>>>> b9cac8ece73d55e93f112cdffe9fb454686442e4
				/*
				- Debugging mode?
				- On debugging exit, stateNum = 2 to go back to STATE_MAIN
				- break;
				*/
				state = state_main;
				break;
<<<<<<< HEAD
=======
			///////////////STATE_DEBUG_END///////////////
>>>>>>> b9cac8ece73d55e93f112cdffe9fb454686442e4
		}
		
		
		
	}
	return 0;
<<<<<<< HEAD
}
=======
}
>>>>>>> b9cac8ece73d55e93f112cdffe9fb454686442e4
