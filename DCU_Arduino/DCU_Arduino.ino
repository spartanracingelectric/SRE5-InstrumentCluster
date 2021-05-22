#include <avr/interrupt.h>
#include "conf.h"
#include "indicators.h"
#include "1602lcd.h"
#include "buttons.h"
#include "can.h"

signed int RPM;
float SOC, TEMP;
bool SOC_blink = false;
bool TEMP_blink = false;

void setup() {
  LCD__init();  // Serial.begin(9600);
  //Serial.begin(9600);
  CAN_initialize();
  buttons__init();

  LCD__wake();
  LCD__default();
  rpm__set(0b1111111111111111);
  left_rgb__set(BLACK);
  right_rgb__set(BLACK);
  left_indicator__set(1); // TURN OFF BLUE LED #1 NOTE THAT 1 = OFF. 0 = ON
  right_indicator__set(1); // TURN OFF BLUE LED #1 NOTE THAT 1 = OFF. 0 = ON
}

void loop() {
  /*
  RPM = (signed int)CAN__receive_RPM();
  SOC = CAN__receive_SOC();
  TEMP = CAN__receive_TEMP();

  Serial.println("******************************************************************************");
  Serial.print("RPM:\t");
  Serial.println(RPM);
  Serial.print("SOC:\t");
  Serial.println(SOC);
  Serial.print("TEMP:\t");
  Serial.println(TEMP);

  indicator__update(RPM, SOC, TEMP, &SOC_blink, &TEMP_blink);
  */
  
  buttons__poll();
  buttons__update_LCD();

  /*
  // Use this stuff to test the leds
  // in this case, 1 = ON; 0 = OFF
  delay(300);
  rpm__set(0b0000000011111111);
  delay(300);
  rpm__set(0b1111111100000000);
  delay(300);

  // 1 = OFF; 0 = ON 
  uint16_t status = 0b1111111111111111;

  for (int i = 0; i < 16; i++) {
    status &= ~(1<<i);
    rpm__set(status);
    delay(100);

  }
  
  for (int i = 16; i >= 0; i--) {
    status |= (1<<i);
    rpm__set(status);
    delay(100);
  } 
  
  delay(300);
  left_indicator__set(0);
  delay(300);
  right_indicator__set(0);
  delay(300);
  left_rgb__set(RED);
  delay(300);
  left_rgb__set(WHITE);
  delay(300);
  left_rgb__set(BLUE);
  delay(300);
  left_rgb__set(GREEN);
  delay(300);
  right_rgb__set(RED);
  delay(300);
  right_rgb__set(WHITE);
  delay(300);
  right_rgb__set(BLUE);
  delay(300);
  right_rgb__set(GREEN);
  delay(300);
  right_rgb__set(BLACK);
  delay(300);
  left_rgb__set(BLACK);
  delay(300);
  left_indicator__set(1);
  delay(300);
  right_indicator__set(1);
  delay(300);
  */
}
