/*
 * indicators.h
 *
 * Created: 10/10/2020 2:25:23 PM
 *  Author: wayne
 */ 


#ifndef INDICATORS_H_
#define INDICATORS_H_

//Color Profiles {G, B, R}
/*
static const uint16_t RED[] = {0, 0, 100};
static const uint16_t ORANGE[] = {2, 0, 32};
static const uint16_t YELLOW[] = {100, 0, 100};
static const uint16_t GREEN[] = {100, 0, 0};
static const uint16_t BLUE[] = {0, 100, 0};
static const uint16_t INDIGO[] = {0, 19, 4};
static const uint16_t VIOLET[] = {0, 9, 9};
static const uint16_t CYAN[] = {100, 100, 0};
static const uint16_t MAGENTA[] = {0, 100, 100};
static const uint16_t WHITE[] = {100, 100, 100};
static const uint16_t BLACK[] = {0, 0, 0};
static const uint16_t PINK[] = {2, 30, 62};
*/

//Color Profiles 0bRBG
//Use if you're not using PWM
//LED indicator bits: 
//L indicator, R indicator, RR, RB, RG, LR, LB, LG 
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

//Indicator Instructions
void timer__init();
//void left_rgb__set(const uint16_t color[]);	PWM
//void right_rgb__set(const uint16_t color[]);	PWM
void left_rgb__set(const uint8_t color);
void right_rgb__set(const uint8_t color);
void left_indicator__set(uint8_t status);
void right_indicator__set(uint8_t status);
void rpm__set(uint16_t LED_PATTERN);
void indicator__update();

#endif /* INDICATORS_H_ */