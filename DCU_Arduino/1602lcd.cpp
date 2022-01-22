#include "1602lcd.h"

uint8_t state = 1;
uint8_t lv_warning = 0;

LiquidCrystal_I2C lcd(LCD_ADDRESS, ROWS, COLS); 

// Initialize the LCD, run in the setup function
void LCD__init() {
  //Serial.begin(9600);
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
  
  LCD__write("HV>", 0, 0);
  LCD__write('<', ROWS-3, 0);
  LCD__write((char)223, ROWS-2, 0);
  LCD__write('C', ROWS-1, 0);
  LCD__write("LV>", 0, 1);
  LCD__write("    <MPH", 8, 1);
}

void LCD__menu() {
  state = 2;
  LCD__clear();
  LCD__write("Stgs.", 0, 0);
  LCD__write("TMaps" , ROWS - 5, 0);
  LCD__write("Back", 0, 1);
  LCD__write("Launch" , ROWS - 6, 1);
  delay(750);
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
  delay(750);
}

void LCD__optionx() {
  state = 4;
  LCD__clear();
  LCD__write("Map1", 0, 0);
  LCD__write("Map2", ROWS-4, 0);
  LCD__write("Back", 0, 1);
  LCD__write("Map3", ROWS-4, 1);
  delay(750);
}

void LCD__option_regen(uint8_t regen_state) {
  state = 5;
  LCD__clear();
  if (regen_state == 0) {
    LCD__write("Turn ON");
  } else {
    LCD__write("Turn OFF");
  }
  LCD__write("Back", 0, 1);
  delay(750);
}

// Take in the SOC and TEMP and update the LCD to display it
void LCD__update(float SpeedMPH, float TEMP, float LV, float HV) {
  if (state == 1) {
    if (LV < LV_WARNING_THRESHOLD) {
      if (lv_warning == 0) {
        LCD__write("STOP LV!", 8, 1);
      }
      lv_warning = 1;
    }
    else {
      if (lv_warning == 1) {
        //Rewrite MPH indicator (can optimize?)
        LCD__write("    <MPH", 8, 1);
      }
      
      //Second row, starting index 10 (XY 2 chars, "<MPH" index is 12-15)
      LCD__write((int)SpeedMPH, 10, 1);

      lv_warning = 0;
    }

    //First row, starting index 3 (WXY.Z 5 chars, "HV>" index is 0-2)
    LCD__write((float)HV, 5, 1, 3, 0);

    //Second row, starting index 3 (XY.Z 4 chars, "LV>" index is 0-2)
    LCD__write((float)LV, 4, 1, 3, 1);

    //First row, starting index 10 (XY 2 chars, "<°C" index is 13-15)
    LCD__write((int)TEMP, 11, 0);

    //Clear MPH ghost digit if MPH is single digit value
    LCD__write(" ", 11, 1);

  }
}
