#include <SPI.h>
#include "conf.h"
#include "mcp_can.h"

#ifndef CAN_H_
#define CAN_H_

#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
    #define SERIAL SerialUSB
#else
    #define SERIAL Serial
#endif

extern MCP_CAN CAN;  

typedef struct CAN_message {
  unsigned long id;
  unsigned char data[8];
} can_message;

void CAN_initialize();
can_message CAN__receive_packet();

float CAN__convert_HV(can_message packet);
float CAN__convert_LV(can_message packet);
signed int CAN__convert_RPM(can_message packet);
float CAN__convert_SOC(can_message packet);
float CAN__convert_TEMP(can_message packet);
void CAN__print_packet(can_message packet);
void CAN__print_recieved_values(signed int RPM, float SOC, float TEMP);
#endif /* CAN_H_ */
