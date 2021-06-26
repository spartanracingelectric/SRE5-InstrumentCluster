#include "1602lcd.h"

uint8_t state = 1;

LiquidCrystal_I2C lcd(LCD_ADDRESS, ROWS, COLS); 

// Initialize the LCD, run in the setup function
void LCD__init() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  Serial.println("LCD init done");
}

// Run for tests
void LCD__wake() {
  for (int i = 0; i < COLS; i++) {
    for (int ii = 0; ii < ROWS; ii++) {
      LCD__write(char(0xFF), ii, i); //Black bars
      delay(15);
    }
  }
  delay(250);
  LCD__clear_row(0);
  LCD__clear_row(1);
  delay(250);
  LCD__write("---- READY! ----", 0, 0);
  for (int i = 0; i < ROWS; i++) {
     LCD__write((char)194, i, 1);
  }
  delay(750);
  LCD__clear(); 
}

/*---------------------------------------------------------------------------/
/ LCD Write functions
/---------------------------------------------------------------------------*/
// Takes in a string to output with a position (x = col y = row)
void LCD__write(String str, int x = 0, int y = 0) {
  lcd.setCursor(x, y);
  lcd.print(str);
}

void LCD__write(char c, int x = 0, int y = 0) {
  lcd.setCursor(x, y);
  lcd.print(c);
}

void LCD__write(int i, int x = 0, int y = 0) {
  lcd.setCursor(x, y);
  lcd.print(i);
}

void LCD__write(float f, int print_len, int dec_places = 1, int x = 0, int y = 0) {
  //Serial.println(f);
  char buff[6];
  lcd.setCursor(x, y);
  dtostrf(f, print_len, dec_places, buff);
  //Serial.println(buff);
  lcd.print(buff);
}

/*---------------------------------------------------------------------------/
/ LCD Clear Functions
/---------------------------------------------------------------------------*/
void LCD__clear() {
  lcd.clear();
}

void LCD__clear_row(int row) {
  for (int i = 0; i < ROWS; i++) {
    lcd.setCursor(i, row);
    lcd.print(" ");
  }
}

/*---------------------------------------------------------------------------/
/ LCD Menu Functions
/---------------------------------------------------------------------------*/
/*
void LCD__timestamp() {
  LCD__write("TMSTMP REC!", 2, 1);
  delay(400);
  LCD__clear_row(1);
}
*/

void LCD__default() {
  state = 1;
  LCD__clear();
  LCD__write("SOC>", 0, 0);
  LCD__write('<', ROWS-3, 0);
  LCD__write((char)223, ROWS-2, 0);
  LCD__write('C', ROWS-1, 0);
  LCD__write("LV>", 0, 1);
  LCD__write("<HV", ROWS-3, 1);
}

void LCD__menu() {
  state = 2;
  LCD__clear();
  LCD__write("Stgs.", 0, 0);
  LCD__write("Op. X" , ROWS - 5, 0);
  LCD__write("Back", 0, 1);
}

void LCD__back() {
  if (state >= 3) {
    LCD__menu();
  } else {
    LCD__default();
  }
}

void LCD__settings() {
  state = 3;
  LCD__clear();
  LCD__write("Settings");
  LCD__write("Back", 0, 1);
}

void LCD__optionx() {
  state = 4;
  LCD__clear();
  LCD__write("Option X");
  LCD__write("Back", 0, 1);
}

void LCD__optiony() {
  state = 5;
  LCD__clear();
  LCD__write("Option Y");
  LCD__write("Back", 0, 1);
}

// Take in the SOC and TEMP and update the LCD to display it
void LCD__update(float SOC, float TEMP, float LV, float HV) {
  if (state == 1) {
    LCD__write((int)SOC, ROWS-12, 0);
    LCD__write((int)TEMP, ROWS - 5, 0);
    LCD__write((float)LV, 4, 1, ROWS-13, 1);
    LCD__write((float)HV, 5, 1, ROWS-8, 1);
  }
}
