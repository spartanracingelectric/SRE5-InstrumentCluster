/*
 * CAN_defs.h
 *
 * Created: 1/30/2021 2:40:36 PM
 *  Author: Waylon
 */ 


#ifndef CAN_DEFS_H_
#define CAN_DEFS_H_

#define RESET_INSTRUCTION 0xC0
#define READ_INSTRUCTION 0x03
#define WRITE_INSTRUCTION 0x02
#define BITMODIFY_INSTRUCTION 0x05

#define CANSTAT 0xE
#define CNF3 0x28
#define CNF2 0x29
#define CNF1 0x2A
#define RXB0CTRL 0x60
#define RXB1CTRL 0x70
#define RXM0 5
#define RXM1 6
#define BUKT 2
#define CANCTRL 0x0F

#define SLEEP_MODE 1
#define LOOPBACK_MODE 2
#define LISTEN_ONLY_MODE 3
#define CONFIGURATION_MODE 4



#endif /* CAN_DEFS_H_ */