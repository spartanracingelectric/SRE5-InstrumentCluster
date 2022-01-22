#ifndef BUTTONS_H_
#define BUTTONS_H_

#include <Arduino.h>
#include <ACAN2515.h>
#include "1602lcd.h"
#include "indicators.h"
#include "conf.h"

extern uint8_t RegenMode, RegenTorqueLimitNm;

void buttons__init();
void buttons__poll();
void buttons__update_LCD(ACAN2515 *can_obj);
void buttons__flag_reset();

#endif /* BUTTONS_H_ */
