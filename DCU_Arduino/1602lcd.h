#include <Wire.h>
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "conf.h"

#ifndef LCD1602_H_
#define LCD1602_H_

extern uint8_t state;

void LCD__init();
void LCD__write(String str, int x = 0, int y = 0);
void LCD__write(char c, int x = 0, int y = 0);
void LCD__write(int i, int x = 0, int y = 0);
void LCD__write(float f, int print_len, int dec_places = 1, int x = 0, int y = 0);
void LCD__clear();
void LCD__clear_row(int row);
void LCD__wake();
void LCD__default();
//void LCD__timestamp();
void LCD__menu();
void LCD__back();
void LCD__settings();
void LCD__optionx();
void LCD__optiony(uint8_t launch_state);
void LCD__update(float SOC, float TEMP, float LV, float HV);

#endif /* LCD1602_H_ */
