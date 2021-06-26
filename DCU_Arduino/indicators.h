#include <Arduino.h>
#include <Wire.h>
#include "conf.h"

#ifndef INDICATORS_H_
#define INDICATORS_H_

// Indicator Instructions
void left_rgb__set(const uint8_t color);
void right_rgb__set(const uint8_t color);
void left_indicator__set(uint8_t status);
void right_indicator__set(uint8_t status);
void rpm__set(uint16_t LED_PATTERN);
void indicator__wake();
void indicator__update();
void indicator__update(signed int RPM, float SOC, float TEMP);
void indicator__timestamp();

#endif /* INDICATORS_H_ */
