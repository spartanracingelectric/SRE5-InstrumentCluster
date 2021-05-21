#include "buttons.h"

const byte button_pins[] = {BUT1, BUT2, BUT3, BUT4};
int button_flag[4];

uint32_t last_debounce_time = 0;

void buttons__init() {
  for (int i = 0; i < 4; i++) {
    pinMode(button_pins[i], INPUT_PULLUP);
  }
}

// Poll the buttons and update the respective button flags
void buttons__poll() {
  int but1_state = digitalRead(BUT1);
  int but2_state = digitalRead(BUT2);
  int but3_state = digitalRead(BUT3);
  int but4_state = digitalRead(BUT4);

  if (millis() - last_debounce_time > DEBOUNCE_TIME) {
    if (but1_state == LOW)
      button_flag[0] = 1;
    else if (but2_state == LOW)
      button_flag[1] = 1;
    else if (but3_state == LOW)
      button_flag[2] = 1;
    else if (but4_state == LOW)
      button_flag[3] = 1;
  } else {
    last_debounce_time = millis();
  }

  for(int i = 0; i < 4; i++) {
    Serial.print(button_flag[i]);
  }
  Serial.println();
}

// Reset the button flags
// Usually only need to call after using the update LCD function
void buttons__flag_reset() {
  for (int i = 0; i < 4; i++) {
    button_flag[i] = 0;
  }
}

// Update the LCD to the correct menu
void buttons__update_LCD() {
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
}
