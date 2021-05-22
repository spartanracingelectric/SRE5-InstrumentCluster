#include <Arduino.h>
#include <Wire.h>
#include "conf.h"

#ifndef INDICATORS_H_
#define INDICATORS_H_

#define blink_const 100

// Color Profiles 0bRBG
// Use if you're not using PWM
// LED indicator bits (what each bit in the byte controls): 
// L indicator, R indicator, RR, RB, RG, LR, LB, LG 
static const uint8_t RED    = 0b011;
static const uint8_t GREEN  = 0b110;
static const uint8_t BLUE   = 0b101;
static const uint8_t PURPLE = 0b001;
static const uint8_t YELLOW = 0b010;
static const uint8_t TEAL   = 0b100;
static const uint8_t WHITE  = 0b000;
static const uint8_t BLACK	= 0b111;

static const uint8_t ON = 0;
static const uint8_t OFF = 1;

static const uint8_t RIGHT_RGB_MASK = 0b11000111;
static const uint8_t LEFT_RGB_MASK = 0b11111000;

// Indicator Instructions
void left_rgb__set(const uint8_t color);
void right_rgb__set(const uint8_t color);
void left_indicator__set(uint8_t status);
void right_indicator__set(uint8_t status);
void rpm__set(uint16_t LED_PATTERN);
void indicator__update();
void indicator__update(signed int RPM, float SOC, float TEMP, bool* SOC_blink, bool* TEMP_blink);

#endif /* INDICATORS_H_ */
