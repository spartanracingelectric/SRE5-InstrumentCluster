#ifndef BUTTONS_H_
#define BUTTONS_H_

#include <Arduino.h>
#include "1602lcd.h"
#include "conf.h"

void buttons__init();
void buttons__poll();
void buttons__update_LCD();
void buttons__flag_reset();

#endif /* BUTTONS_H_ */
