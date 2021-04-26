#ifndef BUTTONS_H_
#define BUTTONS_H_

#define DEFAULT_SCREEN 1
#define MENU_SCREEN 2
#define SETTINGS_SCREEN 3
#define OPTIONX_SCREEN 4
#define OPTIONY_SCREEN 5

#define DEBOUNCE_TIME 50
#define LOCK_TIME 100

#define HOLD_TIME 1000 //Button hold time for bottom buttons on default screen

#include <Arduino.h>
#include "1602lcd.h"
#include "conf.h"

void buttons__init();
void buttons__poll();
void buttons__update_LCD();
void buttons__flag_reset();

#endif /* BUTTONS_H_ */
