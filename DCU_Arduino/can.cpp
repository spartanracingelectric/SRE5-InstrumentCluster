#include "can.h"

extern MCP_CAN CAN(SPI_CS_PIN);  

uint8_t launch_flag = 0;  // 0 = off; 1 = on

// Initialize CAN module, run in the setup function
void CAN_initialize() {
  while (CAN_OK != CAN.begin(CAN_500KBPS)) {            // init can bus : baudrate = 500k
    SERIAL.println("CAN BUS Shield init fail");
    SERIAL.println(" Init CAN BUS Shield again");
    delay(100);
  }
    
  SERIAL.println("CAN BUS Shield init ok!");    
}

// Returns a can message struct with the id and data buffer
can_message CAN__receive_packet() {
    can_message out;
    unsigned char len = 0;
    unsigned char buf[8];

    if (CAN_MSGAVAIL == CAN.checkReceive()) {         // check if data coming
        CAN.readMsgBuf(&len, buf);                    // read data,  len: data length, buf: data buf
        unsigned long canId = CAN.getCanId();
        out.id = canId;
        for(int i = 0; i < len; i++) {
          out.data[i] = buf[i];
        }        
    } else {
      out.id = 0x00;
      for(int i = 0; i < 8; i++) {
        out.data[i] = 0x01;
      }
    }    
    return out;
}

void CAN__transmit_packet(unsigned int id, uint8_t len, uint8_t *data) {
  CAN.sendMsgBuf(id, 0, len, data);
}

void CAN__transmit_one_byte(unsigned int id, uint8_t one_byte) {
  uint8_t data[8] = {0};
  data[0] = one_byte;     //Assign value of pointer to the singular byte
  CAN.sendMsgBuf(id, 0, 1, data);
}

void CAN__transmit_eight_bytes(unsigned int id, uint8_t one_byte) {
  uint8_t data[8] = {0};
  data[0] = one_byte;     //Assign value of pointer to the singular byte
  CAN.sendMsgBuf(id, 0, 8, data);
}

// launch_flag 1 = ON; 0 = OFF
void CAN__toggle_launch() {
  if (launch_flag == 0) {
    launch_flag = 1;
    //CAN__transmit_one_byte((unsigned int)LC_ADDR, launch_flag);
    CAN__transmit_eight_bytes((unsigned int)LC_ADDR, (uint64_t)launch_flag);
  } else {
    launch_flag = 0; 
    //CAN__transmit_one_byte((unsigned int)LC_ADDR, launch_flag);
    CAN__transmit_eight_bytes((unsigned int)LC_ADDR, (uint64_t)launch_flag);
  }
}

void CAN__transmit_torquemap(uint8_t map_num) {
  //CAN__transmit_one_byte((unsigned int)TM_ADDR, map_num);
  CAN__transmit_eight_bytes((unsigned int)TM_ADDR, (uint64_t)map_num);
}

// Transmit packet over CAN to DAQ, DAQ will see marks in data
void CAN__transmit_timestamp() {
  CAN__transmit_one_byte((unsigned int)TS_ADDR, 0xFF); //Induce a big spike in DAQ channel
}

// Input is a can message struct
// Returns the average battery temperature as a signed float 
// The value returned is in Celsius with one decimal of precision
// The CAN packet is big endian
// SOC percentage is located in bytes 0, 1, 2, and 3 (buf[0 - 3]) of address 0x628
float CAN__convert_HV(can_message packet) {
  return ((float)(packet.data[0] | (packet.data[1] << 8))*0.1f);
}

// Input is a can message struct
// Returns the low voltage as an integer
// The CAN packet is big endian (0x50E = 1294, div by 100 to get 12.94v)
// RPM is byte 0 and 1 (buf[0] and buf[1]) of address 0x700
float CAN__convert_LV(can_message packet) {
  //Serial.println((packet.data[0] | (packet.data[1] << 8)) * 0.01f);
  return ((packet.data[1] | (packet.data[0] << 8)) * 0.01f);
}


// Input is a can message struct
// Returns the RPM as a signed integer
// The CAN packet is little endian
// RPM is byte 2 and 3 of address 0x0A5
// Motor RPM
signed int CAN__convert_RPM(can_message packet) {
  return -(packet.data[2] | (packet.data[3] << 8));
}

float CAN__convert_SpeedMPH(can_message packet) {
  uint16_t FL_rpm, FR_rpm;
  FL_rpm = (packet.data[0] | packet.data[1] << 8); //First two bytes
  FR_rpm = (packet.data[2] | packet.data[3] << 8); //Second two bytes

  //Take average of front two, div by 60 to convert denom --> seconds, multiply by circumference in M,
  //and convert m/s to mph
  return (float)((((FL_rpm+FR_rpm)/2.0f)/60.0f) * WHEEL_CIRCUMFERENCE_M * METERS_SEC_TO_MPH);
}

// Input is a can message struct
// Returns the battery state of charge as a signed float 
// The value returned is a percentage with one decimal of precision (Ex: 51.2%)
// The CAN packet is big endian
// SOC percentage is located in bytes 0, 1, 2, and 3 (buf[0 - 3]) of address 0x627
float CAN__convert_SOC(can_message packet) {
  return ((float)(packet.data[0] | (packet.data[1] << 8) | (packet.data[2] << 16) | (packet.data[3] << 32)) * 0.1f);
}

// Input is a can message struct
// Returns the average battery temperature as a signed float 
// The value returned is in Celsius with one decimal of precision
// The CAN packet is big endian
// SOC percentage is located in bytes 0, 1, 2, and 3 (buf[0 - 3]) of address 0x628
float CAN__convert_TEMP(can_message packet) {
  return ((float)(packet.data[0] | (packet.data[1] << 8) | (packet.data[2] << 16) | (packet.data[3] << 32)) * 0.1f);
}

// Input is a can message struct
// Prints the packet info over serial
void CAN__print_packet(can_message packet) {
  // Print the received packet for testing
  Serial.println("-------------------------------");
  Serial.print("Messages at ID: 0x");
  Serial.println(packet.id, HEX);
  for (int i = 0; i < 8; i++) { // print the data
        Serial.print(packet.data[i], HEX);
        Serial.print("\t");
  }
  Serial.println();
}

// Input are the RPM, TEMP, and SOC values
// Prints the RPM, SOC, and TEMP values over serial
void CAN__print_recieved_values(signed int RPM, float SOC, float TEMP) {
  // Print the RPM, SOC, and TEMP values for testing
  Serial.println("******************************************************************************");
  Serial.print("RPM:\t");
  Serial.println(RPM);
  Serial.print("SOC:\t");
  Serial.println(SOC);
  Serial.print("TEMP:\t");
  Serial.println(TEMP);
  Serial.println("******************************************************************************");
  
}
