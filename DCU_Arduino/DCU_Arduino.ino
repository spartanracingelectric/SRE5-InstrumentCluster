#include <avr/interrupt.h>
#include "indicators.h"
#include "1602lcd.h"
#include "buttons.h"

void setup() {
  LCD__init(); 
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
  buttons__poll();
  buttons__update_LCD();
  /*
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
