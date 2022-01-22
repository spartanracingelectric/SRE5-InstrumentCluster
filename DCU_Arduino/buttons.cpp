#include "buttons.h"

const byte button_pins[] = {BUT1, BUT2, BUT3, BUT4};
int button_flag[4];

uint32_t last_debounce_time = 0;
uint8_t regen_flag = 0;  // 0 = off; 1 = on


CANMessage timestamp_frame;
CANMessage regen1_frame;
CANMessage regen2_frame;

uint8_t RegenMode = 0, RegenTorqueLimitNm = 0;

uint8_t regenCanReceived = 0;
uint8_t lRegenMode = 0, lRegenTorqueLimitNm = 0;

// Initialize the buttons, run in the setup function
void buttons__init() {
  for (int i = 0; i < 4; i++) {
    pinMode(button_pins[i], INPUT);
    button_flag[i] = 0;
  }
  Serial.println("Button init done");

  timestamp_frame.id = TS_ADDR;
  timestamp_frame.len = 1;
  timestamp_frame.data[0] = 0x7F;
  regen1_frame.id = REGEN1_ADDR;
  regen1_frame.len = 8;
  regen2_frame.id = REGEN2_ADDR;
  regen2_frame.len = 8;
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
    if (but2_state == LOW)
      button_flag[1] = 1;
    if (but3_state == LOW)
      button_flag[2] = 1;
    if (but4_state == LOW)
      button_flag[3] = 1;
  } else {
    last_debounce_time = millis();
  }

  //Serial.println();
}

// Reset the button flags
// Usually only need to call after using the update LCD function
void buttons__flag_reset() {
  for (int i = 0; i < 4; i++) {
    button_flag[i] = 0;
  }
}

// Update the LCD to the correct menu
void buttons__update_LCD(ACAN2515 *can_obj) {
  switch(state) {
    case DEFAULT_SCREEN: //state = 1
      if (button_flag[0]) {
        indicator__timestamp();
        //CAN__transmit_timestamp();
        can_obj->tryToSend(timestamp_frame);
      }
      else if (button_flag[1]) {
        indicator__timestamp();
        //CAN__transmit_timestamp();
        can_obj->tryToSend(timestamp_frame);
      }
      else if (button_flag[2] && button_flag[3]) {
        LCD__menu();
      }
      //else if (button_flag[3])
        //LCD__menu();
      buttons__flag_reset();
      
      break;
      
    case MENU_SCREEN: //state = 2
      if (button_flag[0])
        LCD__settings();
      else if (button_flag[1]) {
        lRegenMode = RegenMode;
        LCD__regen1(lRegenMode);
      }
      else if (button_flag[2])
        LCD__back();
      else if (button_flag[3]) {
        lRegenTorqueLimitNm = RegenTorqueLimitNm;
        LCD__regen2(lRegenTorqueLimitNm);
      }
      buttons__flag_reset();
      break;
    
    case SETTINGS_SCREEN: //state = 3
      if (button_flag[2])
        LCD__back();
      buttons__flag_reset();
      break;
    
    case REGEN1_SCREEN: //state = 4 (Torque Map Mode)
      if (button_flag[0]) { //Decrease
        if (lRegenMode != 0) { //min 0
          LCD__write(--lRegenMode, ROWS-1, 0);
        }
        delay(200);
      }
      if (button_flag[1]) { //Increase
        if (lRegenMode != 4) { //min 4
          LCD__write(++lRegenMode, ROWS-1, 0);
        }
        delay(200);
      }
      if (button_flag[2]) {
        LCD__back();
      }
      if (button_flag[3]) { //Send
        indicator__blink_bottom();
        regen1_frame.data[0] = lRegenMode;
        can_obj->tryToSend(regen1_frame);
        delay(300);
      }
      buttons__flag_reset();
      break;
    
    case REGEN2_SCREEN: //state = 5
      if (button_flag[0]) { //Decrease
        if (lRegenTorqueLimitNm != 0) { //min 0
          LCD__write("  ", ROWS-1, 0);
          LCD__write(--lRegenTorqueLimitNm, ROWS-3, 0);
        }
        delay(50);
      }
      if (button_flag[1]) { //Increase
        if (lRegenTorqueLimitNm != 180) { //max 80
          LCD__write(++lRegenTorqueLimitNm, ROWS-3, 0);
        }
        delay(50);
      }
      if (button_flag[2]) {
        LCD__back();
      }
      if (button_flag[3]) { //Send
        indicator__blink_bottom();
        regen2_frame.data[0] = lRegenTorqueLimitNm;
        can_obj->tryToSend(regen2_frame);
        delay(300);
      }
      buttons__flag_reset();
      break;
    
  }
}
