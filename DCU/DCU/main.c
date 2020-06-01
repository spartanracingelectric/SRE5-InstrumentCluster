#ifndef F_CPU
#define F_CPU 16000000UL
#endif

///////////////LED Section///////////////
//Indicator LEDs
#define RGB_Addr //LED Indicators Device Address
#define RGB1 1 //Indicator LED 1
#define RGB2 2 //Indicator LED 2
///////////////LED Section///////////////

#include <avr/io.h>	
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/twi.h>
#include "i2c.h"
#include "led.h"

enum state{state_wake, state_main, state_settings, state_debug,};
int main(void) {

	int state = state_wake; //every time the car or instrument cluster turns on, start at STATE_WAKE

	while (1) 
	{
		switch (state)
		{
			//STATE_WAKE
			case 1: //stateNum == 1, wake
				/*
				- Display LCD message to test if LCD works
				- LED patterns and blinks to test they are working too
				- Basically a bunch of statements to test hardware, preferably quick ones to allow for functionality ASAP
				*/
				twi_init();
				timer_Init(); 
				//Blink RPM INDICATOR
				rpm_write(0x0); 
				_delay_ms(300);
				rpm_write(0xFFFF);
				_delay_ms(300);
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
				/*
				- Take in and process CAN packets
				- Record CAN Packet to SD Card
				- Update LCD/LEDs
				- Read for button press to change engine-state
				*/
				state = state_settings;
				break;
			
			
			/**
			//STATE_SETTINGS
			case 3: //stateNum == 3, settings menu
				/*
				- LCD controlled, change settings with buttons (if (buttonPressed) to toggle through and select in a menu)
				- Timeout in case driver accidentally hits button, maybe like 15 seconds -> stateNum = 2 to go back to STATE_MAIN
				- On settings exit, stateNum = 2 to go back to STATE_MAIN
				- break;
				*/
				state = state_debug;
				break;
			
			
			//STATE_DEBUG
			case 4:  //stateNum == 4, debug menu
				/*
				- Debugging mode?
				- On debugging exit, stateNum = 2 to go back to STATE_MAIN
				- break;
				*/
				state = state_main;
				break;
		}
		
		
		
	}
	return 0;
}