#include <SPI.h>
#include "mcp_can.h"

#ifndef CAN_H_
#define CAN_H_


#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
    #define SERIAL SerialUSB
#else
    #define SERIAL Serial
#endif

typedef struct CAN_message {
  unsigned long id;
  unsigned char data[8];
} can_message;

// CAN packet addresses based on the spreadsheet
#define SOC_ADDR 0x627
#define BAT_TEMP_ADDR 0x628
#define RPM_ADDR 0x0A5

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 9;

void CAN_initialize();
//void CAN_receive_SOC();
signed int CAN__receive_RPM();
float CAN__receive_SOC();
float CAN__receive_TEMP();
#endif /* CAN_H_ */
