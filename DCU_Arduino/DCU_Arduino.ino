// #include <ArduinoQueue.h>
#include <avr/interrupt.h>
#include "conf.h"
#include "indicators.h"
#include "1602lcd.h"
#include "buttons.h"
#include "can.h"
#include "mcp_can.h"

// #define QUEUE_MAX_SIZE 50

signed int RPM;
float SOC, TEMP;

int address_counter = 0;

can_message input;
// ArduinoQueue<can_message> messages(QUEUE_MAX_SIZE);

void setup() {
  LCD__init();  // Serial.begin(9600);
  CAN_initialize();
  buttons__init();

  CAN.init_Filt(0, 0, SOC_ADDR);

  LCD__wake();
  LCD__default();

  indicator__wake();
}

void loop() {
  input = CAN__receive_packet();

  // Print the head of the queue for testing
  Serial.println("-------------------------------");
  Serial.print("Messages at ID: 0x");
  Serial.println(input.id, HEX);
  for (int i = 0; i < 8; i++) { // print the data
        Serial.print(input.data[i], HEX);
        Serial.print("\t");
  }
  Serial.println();

  switch (address_counter) {
    case 0: {
      CAN.init_Filt(0, 0, SOC_ADDR);
      address_counter++;
      break;
    }
    case 1: {
      CAN.init_Filt(0, 0, BAT_TEMP_ADDR);
      address_counter++;
      break;
    }
    case 2: {
      CAN.init_Filt(0, 0, RPM_ADDR);
      address_counter++;
      break;
    }
    default: {
      address_counter = 0;
      break;
    }
  }
  
  // If the CAN ID of the queue's head is the 
  // SOC, TEMP, or RPM address, do the proper conversion and dequeue
  // Else, dequeue and continue
  switch (input.id) {
    case SOC_ADDR: {
      SOC = CAN__convert_SOC(input);
      indicator__update(RPM, SOC, TEMP);
      LCD__update(SOC, TEMP);
      break;
    }
    case BAT_TEMP_ADDR: {
      TEMP = CAN__convert_TEMP(input);
      indicator__update(RPM, SOC, TEMP);
      LCD__update(SOC, TEMP);
      break;
    }
    case RPM_ADDR: {  
      RPM = CAN__convert_RPM(input);
      indicator__update(RPM, SOC, TEMP);
      LCD__update(SOC, TEMP);
      break;
    }
    default: {
      break;
    }
  }

  Serial.println("******************************************************************************");
  Serial.print("RPM:\t");
  Serial.println(RPM);
  Serial.print("SOC:\t");
  Serial.println(SOC);
  Serial.print("TEMP:\t");
  Serial.println(TEMP);
  Serial.println("******************************************************************************");

  /*
  // When the queue is full, start processing them all
  if (messages.isFull()) {
    while (messages.itemCount() > 0) {
      // Print the head of the queue for testing
      Serial.println("-------------------------------");
      Serial.print("Messages at ID: 0x");
      Serial.println(messages.getHead().id, HEX);
      for (int i = 0; i < 8; i++) { // print the data
            Serial.print(messages.getHead().data[i], HEX);
            Serial.print("\t");
       }
      Serial.println();  
      
      // If the CAN ID of the queue's head is the 
      // SOC, TEMP, or RPM address, do the proper conversion and dequeue
      // Else, dequeue and continue
      switch (messages.getHead().id) {
      case SOC_ADDR:
        SOC = CAN__convert_SOC(messages.dequeue());
        indicator__update(RPM, SOC, TEMP);
        LCD__update(SOC, TEMP);
        break;
      case BAT_TEMP_ADDR:
        TEMP = CAN__convert_TEMP(messages.dequeue());
        indicator__update(RPM, SOC, TEMP);
        LCD__update(SOC, TEMP);
        break;
      case RPM_ADDR:  
        RPM = CAN__convert_RPM(messages.dequeue());
        indicator__update(RPM, SOC, TEMP);
        LCD__update(SOC, TEMP);
        break;
      default:
        messages.dequeue();
        break;
      } 

      Serial.println("******************************************************************************");
      Serial.print("RPM:\t");
      Serial.println(RPM);
      Serial.print("SOC:\t");
      Serial.println(SOC);
      Serial.print("TEMP:\t");
      Serial.println(TEMP);
      Serial.println("******************************************************************************");
    }
  }

  // Add the CAN packet to the queue
  messages.enqueue(input);

  // Update the LEDs and LCD
  // Then poll the buttons and upadate LCD based on button states
  //buttons__poll();
  //buttons__update_LCD();
  */
  
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

  indicator__update(RPM, SOC, TEMP);
  
  buttons__poll();
  buttons__update_LCD();
  */

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
