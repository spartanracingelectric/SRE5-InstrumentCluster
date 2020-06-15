#ifndef INDLED_H_
#define INDLED_H_

///INDICATOR LED SECTIOn////////////////////////////////////////////////////////
//Indicator LED #1 (ref by RGB1)
int Duty_Cyles1[3] = {100,100,100}; // {R, G, B} 

//Indicator LED #2 (ref by RGB2)
int Duty_Cyles2[3] = {100,100,100}; // {R, G, B}

//Variables used by timer and interrupt
volatile int timerTick =0;
volatile int bitstate = 0x7;

//Specifies color
enum RGB{R,G,B};

//Color Profiles
uint8_t RED[] = {
255, 0, 0};
uint8_t ORANGE[] = {
83, 4, 0};
uint8_t YELLOW[] = {
255, 255, 0};
uint8_t GREEN[] = {
0, 255, 0};
uint8_t BLUE[] = {
0, 0, 255};
uint8_t INDIGO[] = {
4, 0, 19};
uint8_t VIOLET[] = {
23, 0, 22};
uint8_t CYAN[] = {
0, 255, 255};
uint8_t MAGENTA[] = {
255, 0, 255};
uint8_t WHITE[] = {
255, 255, 255};
uint8_t BLACK[] = {
0, 0, 0};
uint8_t PINK[] = {
158, 4, 79};

//Interrupt that pulses at 100khz; 100 pulses = 100hz period
ISR(TIMER0_COMPA_vect)
{
	timerTick++; //Counts interrupt pulses
	if (timerTick >= 100) {
		timerTick = 0;
		bitstate =0b00000000;
		twi_start(0x20);
		twi_write(bitstate); //TURN ON ALL COLORS
		twi_stop();
	}
	for (int i = 0; i<3; i++)
	{
		if (timerTick == Duty_Cyles1[i])
		{
			bitstate |= (1<<i);
			twi_start(0x20);
			twi_write(bitstate); //TURNS OFF SPECIFIED LED
			twi_stop();
		}
		if (timerTick == Duty_Cyles2[i])
		{
			bitstate |= (1<<(i+5)); //IMPORTANT REPLACE 5 WITH THE DISPLACEMENT OF RGB2 BITS FROM THE LEAST SIGNIFICANT BIT
			twi_start(0x20);
			twi_write(bitstate); //TURNS OFF SPECIFIED LED
			twi_stop();
		}
	}
}

//Timer initialization
void timer_Init()
{
	TCCR0A = 0; // set entire TCCR0A register to 0
	TCCR0B = 0; // same for TCCR0B
	TCNT0  = 0; // initialize counter value to 0
	// set compare match register for 100000 Hz increments
	OCR0A = 159; // = 16000000 / (1 * 100000) - 1 (must be <256)
	// turn on CTC mode
	TCCR0B |= (1 << WGM01);
	// Set CS02, CS01 and CS00 bits for 1 prescaler
	TCCR0B |= (0 << CS02) | (0 << CS01) | (1 << CS00);
	// enable timer compare interrupt
	TIMSK0 |= (1 << OCIE0A);
	sei(); // allow interrupts
}

//SETS LED COLOR via THE DUTY CYCLE
void rgb_set(int LED_ID, uint8_t color[])
{
	switch (LED_ID)
	{
		case RGB1:
		for (int i = 0; i < 3; i++)
		{
			Duty_Cyles1[i] = color[i];
		}
		break;
		
		case RGB2:
		for (int i = 0; i < 3; i++)
		{
			Duty_Cyles2[i] = color[i];
		}
		break;
	}
}

//RPM LED SECTION////////////////////////////////////////////////////////

void rpm_write(uint16_t LED_PATTERN)
{
	//1 = off; 0 = on
	uint8_t byte1 = (LED_PATTERN & 0xff), byte2 = (LED_PATTERN >>8);
	twi_init();
	twi_start(0x20);
	twi_write(byte1);
	twi_stop();
	
	twi_start(0x21);
	twi_write(byte2);
	twi_stop();
}

#endif /* INDLED_H_ */