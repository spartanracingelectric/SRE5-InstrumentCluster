//Requires SPI.h and MCP2515_DEFS.h

#ifndef CAN_H_
#define CAN_H_

#include "SPi.h"
#include "MCP2515_DEFS.h"

#define getMode (readRegister(CANSTAT) >> 5)
#define setMode(mode) { changeBits(CANCTRL, (7 << REQOP0), (mode << REQOP0)); while(getMode != mode); }
#define setRollover(v) changeBits(RXB0CTRL, 1 << BUKT, v << BUKT); 



int RXB0_Flag = 0; // 0 when not received, 1 when received.

//read value of register on the selected address in the mcp2515
unsigned char readRegister(unsigned char address)
{
	/* Send read instruction, address, and receive result */
	spiMasterChipSelect(1);
	spiMasterTRANSMIT(READ_INSTRUCTION);
	spiMasterTRANSMIT(address);
	unsigned char buffer = spiMasterTRANSMIT(0);
	spiMasterChipSelect(0);
	return buffer;
}

///Change value of the register on the selected address inside the mcp2515
void writeRegister(unsigned char address, unsigned char value)
{
	/* Send write instruction, address, and data */
	spiMasterChipSelect(1);
	spiMasterTRANSMIT(WRITE_INSTRUCTION);
	spiMasterTRANSMIT(address);
	spiMasterTRANSMIT(value);
	spiMasterChipSelect(0);
}

void changeBits(unsigned char address, unsigned char mask, unsigned char value)
{
	spiMasterChipSelect(1);
	spiMasterTRANSMIT(BITMODIFY_INSTRUCTION);
	spiMasterTRANSMIT(address);
	spiMasterTRANSMIT(mask);
	spiMasterTRANSMIT(value);
	spiMasterChipSelect(0);
	
}

//CAN ONLY SET FILTERS AND MASK IN CONFIG MODE
//THIS IS ONLY FOR STANDARD 11 bit IDs
//Send reset instruction to the MCP2515. Device should reinitialize itself and go to the configuration mode
void resetMCP2515()
{
	/* Send reset instruction */
	spiMasterChipSelect(1);
	spiMasterTRANSMIT(RESET_INSTRUCTION);
	spiMasterChipSelect(0);
}

//masks tell the controller which specific bits of the identifier to check
void setMask01(uint16_t identifier)
{
	//write to RXM0SIDH
	writeRegister(RXM0SIDH, (uint8_t)(identifier>>3));
	writeRegister(RXM0SIDL, (uint8_t)(identifier<<5));
	//Write to RXM1SIDH
	writeRegister(RXM1SIDH, (uint8_t)(identifier>>3));
	writeRegister(RXM1SIDL, (uint8_t)(identifier<<5));
}

//filters tell the controller which of those bits specified by "1" in the mask should be set or unset. Essentially this should be the identifier we want let through.
void setFilter012345(uint16_t identifier)
{
	 writeRegister(RXF0SIDH, (uint8_t)(identifier>>3));
	 writeRegister(RXF0SIDL, (uint8_t)(identifier<<5));
	 
	 writeRegister(RXF1SIDH, (uint8_t)(identifier>>3));
	 writeRegister(RXF1SIDL, (uint8_t)(identifier<<5));
	
	 writeRegister(RXF2SIDH, (uint8_t)(identifier>>3));
	 writeRegister(RXF2SIDL, (uint8_t)(identifier<<5));
	 
	 writeRegister(RXF3SIDH, (uint8_t)(identifier>>3));
	 writeRegister(RXF3SIDL, (uint8_t)(identifier<<5));
	 
	 writeRegister(RXF4SIDH, (uint8_t)(identifier>>3));
	 writeRegister(RXF4SIDL, (uint8_t)(identifier<<5));
	 
	 writeRegister(RXF5SIDH, (uint8_t)(identifier>>3));
	 writeRegister(RXF5SIDL, (uint8_t)(identifier<<5));
}


//CNF1 = 0x01
//CNF2 = 0x91
//CNF3 = 0x01

unsigned char setBitTiming(unsigned char rCNF1, unsigned char rCNF2, unsigned char rCNF3)
{
	if(getMode == CONFIGURATION_MODE) {
		writeRegister(CNF1, rCNF1);
		writeRegister(CNF2, rCNF2);
		changeBits(CNF3, 0x07, rCNF3);
		return 1;
	}
	return 0;
}

void atmegaExtInterruptINIT(void)
{
	/* Initialize external interrupt on pin INT0 on failing edge */
	EICRA |= (1 << ISC01);
	EICRA &= ~(1<<ISC00);
	EIMSK |= (1 << INT0);
}

void mcp2515InterruptINIT(void)
{
	changeBits(CANINTE, 1<<RX0IE, 0x01); //enable receive interrupt on RXB0
	changeBits(CANINTF, 1<<RX1IF, 0x00); //clear receive buffer 1 interrupt flag bit
	changeBits(CANINTF, 1<<RX0IF, 0x00); //clear receive buffer 0 interrupt flag bit
}

ISR(INT0_vect)
{
	changeBits(CANINTF, 1<<RX1IF, 0x00); //clear receive buffer 1 interrupt flag bit
	changeBits(CANINTF, 1<<RX0IF, 0x00); //clear receive buffer 0 interrupt flag bit
	RXB0_Flag = 1;
}

//CNF1 = 0x01
//CNF2 = 0x91
//CNF3 = 0x01
void initMCP2515(void)
{
	/* Initialize SPI as a master device, on frequency < 10Mhz */
	spiMasterINIT();
	/* Initialize external interrupt service on this device */
	atmegaExtInterruptINIT();

	/* Send reset instruction */
	resetMCP2515();
	/* Set configuration mode */
	setMode(CONFIGURATION_MODE);
	/* Set bit timing , masks and rollover mode*/
	setBitTiming(0x01, 0x91, 0x01);
	setMask01(0x0);
	setFilter012345(0x0);
	setRollover(1);
	
	//Initialize mcp2515 receive interrupt
	mcp2515InterruptINIT(); 

	/* Get into normal mode and setup communication */
	setMode(NORMAL_MODE)
}

void send0(uint8_t *data, uint16_t id, uint8_t length)
{
	/*
	//set transmit priority
	changeBits(TXB0CTRL, 1<<TXP0, 0x01);
	changeBits(TXB0CTRL, 1<<TXP1, 0x01);
	
	//set standard ID
	writeRegister(TXB0SIDH, (uint8_t) (id>>3));
	writeRegister(TXB0SIDL, (uint8_t) (id<<5));
	writeRegister(TXB0EID8, 0x0);
	writeRegister(TXB0EID0, 0x0);
	
	//set msg to be data frame and data length
	writeRegister(TXB0DLC, length);
	*/
	 int i =0;
	 spiMasterChipSelect(1);
	 spiMasterTRANSMIT(LOAD_TX_BUF_0_ID);
	 spiMasterTRANSMIT(id >>3);         //identifier high bits
	 spiMasterTRANSMIT(id <<5);          //identifier low bits
	 spiMasterTRANSMIT(0x00);            //extended identifier registers(unused)
	 spiMasterTRANSMIT(0x00);
	 spiMasterTRANSMIT(length);
	 for (i=0;i<length;i++)
	 {                           //load data buffer
		 spiMasterTRANSMIT(data[i]);
	 }
	spiMasterChipSelect(0);
	
	spiMasterChipSelect(1);
	spiMasterTRANSMIT(SEND_TX_BUF_0); //Request to send Instruction for buffer 0
	spiMasterChipSelect(0);
}

void sendCANmsg(unsigned char bi, unsigned long id, char * data, unsigned char prop)
{
	/* Initialize reading of the receive buffer */
	spiMasterChipSelect(1);
	/* Send header and address */
	spiMasterTRANSMIT(WRITE_INSTRUCTION);
	spiMasterTRANSMIT(TXBnCTRL(bi));

	/* Setup message priority */
	spiMasterTRANSMIT(prop >> 6);
	/* Setup standard or extended identifier */
	if(prop & 0x10)
	{
		spiMasterTRANSMIT((unsigned char)(id>>3));
		spiMasterTRANSMIT((unsigned char)(id<<5)|(1<<EXIDE)|((unsigned char)(id>>27)));
		spiMasterTRANSMIT((unsigned char)(id>>19));
		spiMasterTRANSMIT((unsigned char)(id>>11));
	}
	else {
		spiMasterTRANSMIT((unsigned char)(id>>3));
		spiMasterTRANSMIT((unsigned char)(id<<5));
	}
	/* Setup message length and RTR bit */
	spiMasterTRANSMIT((prop & 0x0F) | ((prop & 0x20) ? (1 << RTR) : 0));

	/* Store the message into the buffer */
	for(unsigned char i = 0; i < (prop & 0x0F); i++)
	spiMasterTRANSMIT(data[i]);
	/* Release the bus */
	spiMasterChipSelect(0);
	/* Send request to send */
	spiMasterChipSelect(1);
	spiMasterTRANSMIT(SEND_TX_BUF_0); //Request to send Instruction for buffer 0
	spiMasterChipSelect(0);
}

#endif /* CAN_H_ */