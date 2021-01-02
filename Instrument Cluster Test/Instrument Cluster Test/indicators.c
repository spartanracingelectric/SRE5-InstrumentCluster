/*
 * indicators.c
 *
 * Created: 10/10/2020 2:25:55 PM
 *  Author: wayne
 */ 

 #include <avr/interrupt.h>
 #include "i2c.h"
 #include "indicators.h"

 //Duty cycles for both RGB LEDs used for PWM
 uint8_t left_duty_cyles[3] = {100,100,100}; // {G, B, R}
 uint8_t right_duty_cyles[3] = {100,100,100}; // {G, B, R}

 //RPM bar segments represented as bytes
 uint8_t byte1, byte2;

 //RGB led color
 uint8_t rgb_colors;

 //Update flags
 uint8_t left_indicator_flag = 0;		//(Indicates the state of the indicator led - 0  = ON, 1 = OFF)
 uint8_t right_indicator_flag = 0;		//(Indicates the state of the indicator led - 0  = ON, 1 = OFF)
 volatile uint8_t rpm_flag = 0;			//(1 indicates update is queued)
 volatile uint8_t left_rgb_flag = 0;	//(1 indicates update is queued)
 volatile uint8_t right_rgb_flag = 0;	//(1 indicates update is queued)

 //Variables used by timer and interrupt
 volatile uint8_t timerTick = 0;
 //volatile uint8_t bitstate = 0x7;
 uint8_t bitstate = 0b11111111;

 
 //Not PWM Code
 void left_rgb__set(const uint8_t color) {
	 bitstate = (bitstate &= LEFT_RGB_MASK) | color;
	 indicator__update();
 }

 void right_rgb__set(const uint8_t color) {
	 bitstate = (bitstate &= RIGHT_RGB_MASK) | (color << 3);
	 indicator__update();
 }

 void left_indicator__set(uint8_t status) {
	 bitstate = (bitstate & ~(1 << 7)) | (status << 7);
	 indicator__update();
 }

 void right_indicator__set(uint8_t status) {
	 bitstate = (bitstate & ~(1 << 6)) | (status << 6);
	 indicator__update();
 }

 void indicator__update() {
	 i2c__start(LED_INDICATORS);
	 i2c__write(bitstate);
	 i2c__stop();
 }

 void rpm__set(uint16_t LED_PATTERN) {
	 byte1 = (LED_PATTERN & 0xff), byte2 = (LED_PATTERN >>8);
	 i2c__start(LED_BAR_1);
	 i2c__write(byte2);
	 i2c__stop();
	 
	 i2c__start(LED_BAR_2);
	 i2c__write(byte1);
	 i2c__stop();
 }

 //Everything after this is for PWM (Delete eventually)
 //Interrupt that pulses at 10khz; 100 pulses = 100hz period
 ISR(TIMER1_COMPA_vect){
	 //1 = off; 0 = on
	 if (rpm_flag == 1){
		 i2c__start(LED_BAR_1);
		 i2c__write(byte2);
		 i2c__stop();
		 
		 i2c__start(LED_BAR_2);
		 i2c__write(byte1);
		 i2c__stop();
		 rpm_flag = 0;
	 }

	 timerTick++;
	 if (timerTick >= 100){
		 timerTick = 0;
		 bitstate =0b00000000;
		 for (uint8_t i = 0; i<3; i++){
			 if (left_duty_cyles[i] == 0){
				 bitstate |= (1<<i);
			 }
		 }
		 for (uint8_t i = 0; i<3; i++){
			 if (right_duty_cyles[i] == 0){
				 bitstate |= (1<<(i+3));
			 }
		 }
		 
		 if (left_indicator_flag == 1){
			 bitstate |= (1<<6);
		 }
		 if (right_indicator_flag == 1){
			 bitstate |= (1<<7);
		 }
		 i2c__start(LED_INDICATORS);
		 i2c__write(bitstate);
		 i2c__stop();
	 }
	 for (uint8_t i = 0; i<3; i++){
		 if (timerTick == left_duty_cyles[i]){
			 bitstate |= (1<<i);
			 i2c__start(LED_INDICATORS);
			 i2c__write(bitstate);
			 i2c__stop();
		 }
		 if (timerTick == right_duty_cyles[i]){
			 bitstate |= (1<<(i+3));
			 i2c__start(LED_INDICATORS);
			 i2c__write(bitstate);
			 i2c__stop();
		 }
	 }
 }

 //Only needed if you're using PWM
 void timer__init(){
	 // TIMER 1 for interrupt frequency 10000 Hz:
	 cli();
	 TCCR1A = 0;
	 TCCR1B = 0;
	 TCNT1  = 0;
	 OCR1A = 1999;
	 TCCR1B |= (1 << WGM12);
	 TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10);
	 TIMSK1 |= (1 << OCIE1A);
	 sei();
 }

 //PWM functions
 /*
 void left_rgb__set(const uint16_t color[]){
	 for (uint8_t i = 0; i < 3; i++){
		 left_duty_cyles[i] = color[i];
	 }
 }
 void right_rgb__set(const uint16_t color[]){
	 for (uint8_t i = 0; i < 3; i++){
		 right_duty_cyles[i] = color[i];
	 }
 }
 
 void left_indicator__set(uint8_t status){
	 left_indicator_flag = status;
 }

 void right_indicator__set(uint8_t status){
	 right_indicator_flag = status;
 }

 void rpm__set(uint16_t LED_PATTERN){
	 byte1 = (LED_PATTERN & 0xff), byte2 = (LED_PATTERN >>8);
	 rpm_flag = 1;
 }
 */
