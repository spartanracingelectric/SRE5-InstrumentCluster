#include <avr/interrupt.h>
#include "conf.h"
#include "indicators.h"
#include "1602lcd.h"
#include "buttons.h"
#include "can.h"
#include "mcp_can.h"

signed int RPM;
float SOC, TEMP;

int address_counter = 0;

can_message input;

void setup() {
  LCD__init();        // Serial.begin(9600) is included in this function;
  CAN_initialize();   
  // buttons__init();    We won't be using the buttons for now

  // initialize the first filter to search for SOC
  CAN.init_Filt(0, 0, SOC_ADDR);

  LCD__wake();
  LCD__default();

  indicator__wake();
}

void loop() {
indicator__update(5500, 10, 50);
/*
  input = CAN__receive_packet();

  // Print recieved packet's data
  // use for testing, leave commented out during real use
  // CAN__print_packet(input);

  // DYNAMIC CAN ADDRESS FILTERS
  switch (address_counter) {
    case 0: {
      CAN.init_Filt(0, 0, SOC_ADDR);      // Search for SOC
      address_counter++;
      break;
    }
    case 1: {
      CAN.init_Filt(0, 0, BAT_TEMP_ADDR); // Search for TEMP
      address_counter++;
      break;
    }
    case 2: {
      CAN.init_Filt(0, 0, RPM_ADDR);      // Search for RPM
      address_counter++;
      break;
    }
    default: {
      address_counter = 0;
      break;
    }
  }
  
  // If the CAN ID of the recieved packet is the 
  // SOC, TEMP, or RPM address, do the proper conversion
  // Else, do nothing (this should never happen because of the filters)
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
*/
  // Print the RPM, SOC, and TEMP
  // use for testing, leave commented out during real use
  // CAN__print_recieved_values(RPM, SOC, TEMP);
}
