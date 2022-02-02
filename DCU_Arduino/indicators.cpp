#include "indicators.h"

uint32_t BLINK_INTERVAL = 250;

// RPM bar segments represented as bytes
uint8_t byte1, byte2;

// Byte that controls the LED indicators
// L indicator, R indicator, RR, RB, RG, LR, LB, LG 
uint8_t bitstate = 0b11111111;

// Blink variables
uint32_t last_blink = 0;

// If SOC_blink or TEMP_blink are true, that means turn off the led
bool TEMP_blink = false;
bool SOC_blink = false;

/**************************************************************/
// RGB set functions
// color input are the three bits controlling the RGB pins
// refer to indicators.h to see color constants
void left_rgb__set(const uint8_t color) {
  bitstate = (bitstate &= LEFT_RGB_MASK) | color;
  indicator__update();
}

void right_rgb__set(const uint8_t color) {
  bitstate = (bitstate &= RIGHT_RGB_MASK) | (color << 3);
  indicator__update();
}

/**************************************************************/
// Indicator set functions
// 0 = ON 1 = OFF
void left_indicator__set(uint8_t status) {
  bitstate = (bitstate & ~(1 << 7)) | (status << 7);
  indicator__update();
}

void right_indicator__set(uint8_t status) {
  bitstate = (bitstate & ~(1 << 6)) | (status << 6);
  indicator__update();
}

/**************************************************************/
// Updates the RPM bar 
// This is a bit weird, sometimes 1 = ON and 0 = OFF
// Most cases 0 = On and 1 = OFF
void rpm__set(uint16_t led_pattern) {
  byte1 = (led_pattern & 0xff), byte2 = (led_pattern >>8);
  Wire.beginTransmission(LED_BAR_1);
  Wire.write(byte2);
  Wire.endTransmission();

  Wire.beginTransmission(LED_BAR_2);
  Wire.write(byte1);
  Wire.endTransmission();
}

/**************************************************************/
// Run this function during setup to test the leds
void indicator__wake() {
  // Use this stuff to test the leds
  // in this case, 1 = ON; 0 = OFF
  delay(100);
  rpm__set(0b0000000011111111);
  delay(100);
  rpm__set(0b1111111100000000);
  delay(100);

  // 1 = OFF; 0 = ON 
  uint16_t status = 0b1111111111111111;

  for (int i = 0; i < 16; i++) {
    status &= ~(1<<i);
    rpm__set(status);
    delay(50);

  }
  
  for (int i = 16; i >= 0; i--) {
    status |= (1<<i);
    rpm__set(status);
    delay(50);
  } 

  delay(50);
  left_indicator__set(0);
  delay(50);
  right_indicator__set(0);
  delay(50);
  left_rgb__set(RED);
  delay(50);
  left_rgb__set(WHITE);
  delay(50);
  left_rgb__set(BLUE);
  delay(50);
  left_rgb__set(GREEN);
  delay(50);
  right_rgb__set(RED);
  delay(50);
  right_rgb__set(WHITE);
  delay(50);
  right_rgb__set(BLUE);
  delay(50);
  right_rgb__set(GREEN);
  delay(50);
  right_rgb__set(BLACK);
  delay(50);
  left_rgb__set(BLACK);
  delay(50);
  left_indicator__set(1);
  delay(50);
  right_indicator__set(1);
  delay(50);
  
  rpm__set(0b1111111111111111);
  left_rgb__set(BLACK);
  right_rgb__set(BLACK);
  left_indicator__set(1);   // NOTE THAT 1 = OFF. 0 = ON
  right_indicator__set(1);  // NOTE THAT 1 = OFF. 0 = ON
}


/**************************************************************/
// LED update functions
// Updates the status of the LED indicators using i2c
void indicator__update() {
  Wire.beginTransmission(LED_INDICATORS);
  Wire.write(bitstate);
  Wire.endTransmission();
}

// Updates all the LEDS based on the information received from the CAN packets
void indicator__update(signed int RPM, float SOC, float TEMP) {
  uint16_t led_pattern = 0xFFFF;
  uint8_t SOC_color;
  uint8_t TEMP_color;
  
  // Under the assumption that the RPM values will go from 0 to 5500
  // This will also evenly space out the cut off points
  // To change the range, change the second or third argument
  // This integer will indicate how many leds from the RPM bar will be lit
  int bar_num;
  if (RPM > 20)
      bar_num = (int) map(RPM, 20, 3020, 1, 16);
  else
      bar_num = 0;
  for(int i = 0; i < bar_num; i++) {
    led_pattern &= ~(1 << i); 
    //Serial.println(led_pattern, BIN);
  }

  // State of charge will be given as a percentage
  if (SOC > 75) {
    SOC_color = BLACK;
  } else if (SOC <= 75 && SOC > 50) {
    SOC_color = GREEN;
  } else if (SOC <= 50 && SOC > 10) {
    SOC_color = BLUE;
  } else if (SOC <= 10 && SOC > 5) {
    SOC_color = RED;
  } else if (SOC <= 5) {
    if (!SOC_blink) {
      Serial.println("SOC help");
      SOC_color = RED;
      SOC_blink = true;
      last_blink = millis();
    }
  }

  // Battery temperatures are given in celsius
  if (TEMP < 20) {
    TEMP_color = BLACK;
  } else if (TEMP >= 20 && TEMP < 30) {
    TEMP_color = GREEN;
  } else if (TEMP >= 30 && TEMP < 50) {
    TEMP_color = BLUE;
  } else if (TEMP >= 50 && TEMP < 55) {
    TEMP_color = RED;
  } else if (TEMP >= 55) {
    if (!TEMP_blink) {
      Serial.println("TEMP help");
      TEMP_color = RED;
      TEMP_blink = true;
      last_blink = millis();
    }
  }

  Serial.println(millis() - last_blink);
  if (millis() - last_blink > BLINK_INTERVAL) {
    if (SOC_blink && TEMP_blink) {
      SOC_color = BLACK;
      TEMP_color = BLACK;
      SOC_blink = false;
      TEMP_blink = false;
      last_blink = millis();
    } else if (SOC_blink && !TEMP_blink) {
      SOC_color = BLACK;
      SOC_blink = false;
      last_blink = millis();
    } else if (!SOC_blink && TEMP_blink) {
      TEMP_color = BLACK;
      TEMP_blink = false;
      last_blink = millis();
    } else {
      // Do nothing
    }
  }

  Serial.print("SOC blink: ");
  Serial.println(SOC_blink);
  Serial.print("TEMP blink: ");
  Serial.println(TEMP_blink);

  rpm__set(led_pattern);
  //left_rgb__set(SOC_color);
  //right_rgb__set(TEMP_color);
}

void indicator__timestamp() {
  left_indicator__set(0);
  right_indicator__set(0);
  delay(75);
  left_indicator__set(1);
  right_indicator__set(1);
  delay(75);
  left_indicator__set(0);
  right_indicator__set(0);
  delay(75);
  left_indicator__set(1);
  right_indicator__set(1);
}

void indicator__blink_bottom() {
  left_indicator__set(0);
  right_indicator__set(0);
  delay(125);
  left_indicator__set(1);
  right_indicator__set(1);
}
