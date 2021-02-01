#ifndef SPI_H_
#define SPI_H_

#define DDR_SPI DDRB
#define PORT_SPI PORTB
#define PIN_MOSI PINB3
#define PIN_MISO PINB4
#define PIN_SCK PINB5
#define PIN_SS PINB2

void spiMasterINIT(void)
{
    DDR_SPI |= (1 << PIN_SS) | (1 << PIN_MOSI) | (1 << PIN_SCK);
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0);
}


//Transmitting Data - For receiving and sending
//Data Transmitted from zero index
uint8_t spiMasterTRANSMIT(uint8_t data)
{
	/* Start transmission */
	SPDR = data;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	
	return SPDR;
}

// This function is used for setting of the CS pin. CS signal is inverted, so input 1 (true) means zero on the output.
void spiMasterChipSelect(uint8_t state)
{
	/* What the user wants? (remember that the CS signal is inverted) */
	if(!state) {
		/* Raise the CS pin */
		PORT_SPI |= (1<<PIN_SS);
		} else {
		/* Lower the CS pin */
		PORT_SPI &= ~(1<<PIN_SS);
	}
}



#endif /* SPI_H_ */