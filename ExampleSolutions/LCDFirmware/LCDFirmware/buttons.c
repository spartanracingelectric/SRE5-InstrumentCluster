#include "buttons.h"

#define DEFAULT_SCREEN 1
#define MENU_SCREEN 2
#define SETTINGS_SCREEN 3
#define OPTIONX_SCREEN 4
#define OPTIONY_SCREEN 5

#define DEBOUNCE_TIME 20
#define LOCK_TIME 100

void buttons_init() {
	PCMSK1 |= PCINT_BUT1 | PCINT_BUT2 | PCINT_BUT3 | PCINT_BUT4; //Enable pin change interrupts on pc0 -> pc3 pins ( Pin Change Mask Register 1 )
	PCICR |= (1<<PCIE1);  //When the PCIE1 bit is set (one) and the I-bit in the status register (SREG) is set (one), pin change interrupt 1 is enabled ( Pin Change Interrupt Control Register )
}

ISR(PCINT1_vect) {
	
	int button_flag[4];
	
	uint16_t timer;

	void button_flag_reset() {
		for (int i = 0; i < 4; i++)
		button_flag[i] = 0;
	}
	
	timer = 0;
	while (!(PINC & (1<<BUT1))) {
		timer++;
		_delay_ms(1);
	}
	if (timer > DEBOUNCE_TIME)
	button_flag[0] = 1;
	
	timer = 0;
	while (!(PINC & (1<<BUT2))) {
		timer++;
		_delay_ms(1);
	}
	if (timer > DEBOUNCE_TIME)
	button_flag[1] = 1;
	
	timer = 0;
	while (!(PINC & (1<<BUT3))) {
		timer++;
		_delay_ms(1);
	}
	if (timer > DEBOUNCE_TIME) {
		if (state != 1)
		button_flag[2] = 1;
		else if (timer >= HOLD_TIME)
		button_flag[2] = 1;
	}
	
	timer = 0;
	while (!(PINC & (1<<BUT4))) {
		timer++;
		_delay_ms(1);
	}
	if (timer > DEBOUNCE_TIME) {
		if (state != 1)
		button_flag[3] = 1;
		else if (timer >= HOLD_TIME)
		button_flag[3] = 1;
	}
	
	switch(state) {
		case DEFAULT_SCREEN: { //state = 1
			if (button_flag[0])
			LCD_timestamp();
			else if (button_flag[1])
			LCD_timestamp();
			else if (button_flag[2])
			LCD_menu();
			else if (button_flag[3])
			LCD_menu();
			button_flag_reset();
			
		}
		case MENU_SCREEN: { //state = 2
			if (button_flag[0])
			LCD_settings();
			else if (button_flag[1])
			LCD_optionx();
			else if (button_flag[2])
			LCD_back();
			else if (button_flag[3])
			LCD_optiony();
			button_flag_reset();
		}
		case SETTINGS_SCREEN: { //state = 3
			if (button_flag[2])
			LCD_back();
			button_flag_reset();
		}
		case OPTIONX_SCREEN: { //state = 4
			if (button_flag[2])
			LCD_back();
			button_flag_reset();
		}
		case OPTIONY_SCREEN: { //state = 5
			if (button_flag[2])
			LCD_back();
			button_flag_reset();
		}
	}
	
	_delay_ms(LOCK_TIME);
}