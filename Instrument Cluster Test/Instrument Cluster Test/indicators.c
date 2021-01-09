/*
 * indicators.c
 *
 * Created: 10/10/2020 2:25:55 PM
 *  Author: wayne
 */ 

#include <avr/interrupt.h>
#include "i2c.h"
#include "indicators.h"

//RPM bar segments represented as bytes
uint8_t byte1, byte2;

//Byte that controls the LED indicators
//L indicator, R indicator, RR, RB, RG, LR, LB, LG 
uint8_t bitstate = 0b11111111;

//RGB set functions
//color input are the three bits controlling the RGB pins
//refer to indicators.h to see color constants
void left_rgb__set(const uint8_t color) {
	bitstate = (bitstate &= LEFT_RGB_MASK) | color;
	indicator__update();
}

void right_rgb__set(const uint8_t color) {
	bitstate = (bitstate &= RIGHT_RGB_MASK) | (color << 3);
	indicator__update();
}

//Indicator set functions
//0 = ON 1 = OFF
void left_indicator__set(uint8_t status) {
	bitstate = (bitstate & ~(1 << 7)) | (status << 7);
	indicator__update();
}

void right_indicator__set(uint8_t status) {
	bitstate = (bitstate & ~(1 << 6)) | (status << 6);
	indicator__update();
}

//Updates the status of the LED indicators using i2c
void indicator__update() {
	i2c__start(LED_INDICATORS);
	i2c__write(bitstate);
	i2c__stop();
}

//Updates the RPM bar 
//This is a bit weird, sometimes 1 = ON and 0 = OFF
//Most cases 0 = On and 1 = OFF
void rpm__set(uint16_t LED_PATTERN) {
	byte1 = (LED_PATTERN & 0xff), byte2 = (LED_PATTERN >>8);
	i2c__start(LED_BAR_1);
	i2c__write(byte2);
	i2c__stop();
	 
	i2c__start(LED_BAR_2);
	i2c__write(byte1);
	i2c__stop();
}