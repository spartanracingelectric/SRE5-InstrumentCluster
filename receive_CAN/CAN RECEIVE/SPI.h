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
	/* Set MOSI and SCK output, all others input */
	DDR_SPI = (1<<PIN_MOSI)|(1<<PIN_SCK)|(1<<PIN_SS);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR);	SPSR = (1<<SPI2X);
}


//Transmitting Data - For receiving and sending
//Data Transmitted from zero index
unsigned char spiMasterTRANSMIT(unsigned char data)
{
	/* Start transmission */
	SPDR = data;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	
	return SPDR;
}

// This function is used for setting of the CS pin. CS signal is inverted, so input 1 (true) means zero on the output.
void spiMasterChipSelect(unsigned char state)
{
	/* What the user wants? (remember that the CS signal is inverted) */
	if(!state) {
		/* Raise the CS pin */
		PORT_SPI |= (1<<PIN_SS);
		DDR_SPI |= (1<<PIN_SS);
		} else {
		/* Lower the CS pin */
		PORT_SPI &= ~(1<<PIN_SS);
		DDR_SPI |= (1<<PIN_SS);
	}
}



#endif /* SPI_H_ */