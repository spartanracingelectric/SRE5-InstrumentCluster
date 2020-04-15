#ifndef F_CPU
#define F_CPU 16000000UL
#endif

LCD needs SOC and Temp

#include <avr/io.h>

//#define BUTTON1

int main(void) {

	int stateNum = 1; //every time the car or instrument cluster turns on, start at STATE_WAKE

	while (1) {
		switch (stateNum) {
			
			//STATE_WAKE
			case 1: //stateNum == 1, wake
				/*
				- Display LCD message to test if LCD works
				- LED patterns and blinks to test they are working too
				- Basically a bunch of statements to test hardware, preferably quick ones to allow for functionality ASAP
				- stateNum = 2;
				- break;
				*/
				
				
			//STATE_MAIN
			case 2: //stateNum == 2, main screen
				/*
				- Take in and process CAN packets
				- Record CAN Packet to SD Card
				- Update LCD/LEDs
				- Read for button press to change engine-state
				*/
			
			
			/**
			//STATE_SETTINGS
			case 3: //stateNum == 3, settings menu
				/*
				- LCD controlled, change settings with buttons (if (buttonPressed) to toggle through and select in a menu)
				- Timeout in case driver accidentally hits button, maybe like 15 seconds -> stateNum = 2 to go back to STATE_MAIN
				- On settings exit, stateNum = 2 to go back to STATE_MAIN
				- break;
				*/
			
			
			//STATE_DEBUG
			case 4:  //stateNum == 4, debug menu
				/*
				- Debugging mode?
				- On debugging exit, stateNum = 2 to go back to STATE_MAIN
				- break;
				*/
			**/
			
		}
	}
}