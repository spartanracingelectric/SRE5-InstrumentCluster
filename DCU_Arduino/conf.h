#ifndef CONF_H_
#define CONF_H_

// Set to 1 if you want to print the input data over serial
#define SERIAL_PRINT_TESTS 0

/****************************************************************************/
/*  INSTRUMENT CLUSTER - SETUP CONFIGURATION FILE (for ports, etc)			*/
/*																			                                */
/*  DUE TO F_CPU, conf.h MUST BE INCLUDED BEFORE ANY <avr/delay.h>			*/
/****************************************************************************/
/* ATMega328P CLOCK SPEED */
#define F_CPU 16000000UL

/*---------------------------------------------------------------------------/
/ I2C ADDRESSES
/---------------------------------------------------------------------------*/
/* 1602 LCD I2C ADDRESS */
#define LCD_ADDRESS 0x38		

/* RPM LED BAR I2C ADDRESS */
#define LED_BAR_1 0x3A
#define LED_BAR_2 0x39

/* INDICATOR LEDS I2C ADDRESS */
#define LED_INDICATORS 0x3B

/*---------------------------------------------------------------------------/
/ CAN ADDRESSES
/---------------------------------------------------------------------------*/
/* CAN packet addresses based on the spreadsheet */
#define SOC_ADDR 0x627
#define BAT_TEMP_ADDR 0x628
#define RPM_ADDR 0x0A5
#define LV_ADDR 0x700
#define HV_ADDR 0x625
#define TS_ADDR 0x701       //Timestamp Address (-> DAQ)
#define TM_ADDR 0x702       //Torque Mapping Select Address  (-> VCU)

/*---------------------------------------------------------------------------/
/ I/O PORTS
/---------------------------------------------------------------------------*/
/* SPI PINS */
// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 9; 

/*---------------------------------------------------------------------------/
/ BUTTON PINS AND CONSTANTS
/---------------------------------------------------------------------------*/
#define BUT1 14 //PINC0
#define BUT2 15 //PINC1
#define BUT3 16 //PINC2
#define BUT4 17 //PINC3

#define PCINT_BUT1 (1<<PCINT8)
#define PCINT_BUT2 (1<<PCINT9)
#define PCINT_BUT3 (1<<PCINT10)
#define PCINT_BUT4 (1<<PCINT11)

#define DEFAULT_SCREEN 1
#define MENU_SCREEN 2
#define SETTINGS_SCREEN 3
#define OPTIONX_SCREEN 4
#define OPTIONY_SCREEN 5

#define DEBOUNCE_TIME 150
#define LOCK_TIME 100

#define HOLD_TIME 1000 //Button hold time for bottom buttons on default screen

/*---------------------------------------------------------------------------/
/ LCD CONSTANTS
/---------------------------------------------------------------------------*/
/* LCD size */
#define ROWS 16
#define COLS 2

/*---------------------------------------------------------------------------/
/ LED CONSTANTS
/---------------------------------------------------------------------------*/
/* If the RGB LEDS blink, they will blink every 'X' ms */
//#define BLINK_INTERVAL 250  

// Color Profiles 0bRBG
// Use if you're not using PWM
// LED indicator bits (what each bit in the byte controls): 
// L indicator, R indicator, RR, RB, RG, LR, LB, LG 
#define RED     0b011
#define GREEN   0b110
#define BLUE    0b101
#define PURPLE  0b001
#define YELLOW  0b010
#define TEAL    0b100
#define WHITE   0b000
#define BLACK   0b111

#define ON   0
#define OFF  1

#define RIGHT_RGB_MASK  0b11000111
#define LEFT_RGB_MASK   0b11111000

#endif /* CONF_H_ */
