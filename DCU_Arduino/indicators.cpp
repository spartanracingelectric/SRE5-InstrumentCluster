#include "indicators.h"

#define BLINK_INTERVAL 250  // If the RGB LEDS blink, they will blink every 'X' ms

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
  int bar_num = (int) map(RPM, 0, 5500, 0, 16);
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

  rpm__set(led_pattern);
  left_rgb__set(SOC_color);
  right_rgb__set(TEMP_color);
}
