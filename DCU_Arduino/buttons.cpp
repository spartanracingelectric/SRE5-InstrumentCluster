#include "buttons.h"

volatile int button_flag[4];
volatile uint16_t timer;

void buttons__init() {
  Serial.begin(9600);
  PCMSK1 |= PCINT_BUT1 | PCINT_BUT2 | PCINT_BUT3 | PCINT_BUT4; //Enable pin change interrupts on pc0 -> pc3 pins ( Pin Change Mask Register 1 )
  PCICR |= (1<<PCIE1);  //When the PCIE1 bit is set (one) and the I-bit in the status register (SREG) is set (one), pin change interrupt 1 is enabled ( Pin Change Interrupt Control Register )
  Serial.println("Buttons init done");
  for(int i = 0; i < 4; i++) {
    Serial.print(button_flag[i]);
  }
  Serial.println();
}

void buttons__flag_reset() {
  for (int i = 0; i < 4; i++)
    button_flag[i] = 0;
}

ISR(PCINT1_vect) {
  timer = 0;
  while (!(PINC & (1<<BUT1))) {
    timer++;
    delay(1);
  }
  if (timer > DEBOUNCE_TIME) {
    button_flag[0] = 1;
    for(int i = 0; i < 4; i++) {
      Serial.print(i);
    }
    Serial.println();
  }
  
  timer = 0;
  while (!(PINC & (1<<BUT2))) {
    timer++;
    delay(1);
  }
  if (timer > DEBOUNCE_TIME) {
    button_flag[1] = 1;
    for(int i = 0; i < 4; i++) {
      Serial.print(i);
    }
    Serial.println();
  }
    
  timer = 0;
  while (!(PINC & (1<<BUT3))) {
    timer++;
    delay(1);
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
    delay(1);
  }
  if (timer > DEBOUNCE_TIME) {
    if (state != 1)
      button_flag[3] = 1;
    else if (timer >= HOLD_TIME)
      button_flag[3] = 1;
  }

  switch(state) {
    case DEFAULT_SCREEN: //state = 1
      if (button_flag[0])
        LCD__timestamp();
      else if (button_flag[1])
        LCD__timestamp();
      else if (button_flag[2])
        LCD__menu();
      else if (button_flag[3])
        LCD__menu();
      buttons__flag_reset();
      break;
      
    case MENU_SCREEN: //state = 2
      if (button_flag[0])
        LCD__settings();
      else if (button_flag[1])
        LCD__optionx();
      else if (button_flag[2])
        LCD__back();
      else if (button_flag[3])
        LCD__optiony();
      buttons__flag_reset();
      break;
    
    case SETTINGS_SCREEN: //state = 3
      if (button_flag[2])
        LCD__back();
      buttons__flag_reset();
      break;
    
    case OPTIONX_SCREEN: //state = 4
      if (button_flag[2])
        LCD__back();
      buttons__flag_reset();
      break;
    
    case OPTIONY_SCREEN: //state = 5
      if (button_flag[2])
        LCD__back();
      buttons__flag_reset();
      break;
  }

  delay(LOCK_TIME);
}
