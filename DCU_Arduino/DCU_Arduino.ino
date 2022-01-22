#include <avr/interrupt.h>
#include <ACAN2515.h>
#include "conf.h"
#include "indicators.h"
#include "1602lcd.h"
#include "buttons.h"
//#include "can.h"
//#include "mcp_can.h"

static const byte MCP2515_CS  = 9 ; // CS input of MCP2515 (adapt to your design) 
static const byte MCP2515_INT =  2 ; // INT output of MCP2515 (adapt to your design)

ACAN2515 can (MCP2515_CS, SPI, MCP2515_INT);

static const uint32_t QUARTZ_FREQUENCY = 16UL * 1000UL * 1000UL ; // 16 MHz

signed int RPM;
float SpeedMPH=0.0f, TEMP=0.0f, LV=0.0f, HV=0.0f;

int address_counter = 0;

CANMessage *can_ptr;

static void rpm_rx (const CANMessage & inMessage) {
  RPM = -(inMessage.data[2] | (inMessage.data[3] << 8));
  Serial.println ("Receive 0") ;
}

static void lv_rx (const CANMessage & inMessage) {
  LV = ((inMessage.data[1] | (inMessage.data[0] << 8)) * 0.01f) + LV_OFFSET;
  Serial.println ("Receive 1") ;
}

static void hv_rx (const CANMessage & inMessage) {
  HV = ((inMessage.data[0] | (inMessage.data[1] << 8))*0.1f);
  Serial.println ("Receive 2") ;
}

static void temp_rx (const CANMessage & inMessage) {
  TEMP = ((inMessage.data[0] | (inMessage.data[1] << 8) | (inMessage.data[2] << 16) | (inMessage.data[3] << 32)) * 0.1f);
  Serial.println ("Receive 3") ;
}

static void wss_rx (const CANMessage & inMessage) {
  uint16_t FL_rpm, FR_rpm;
    FL_rpm = (inMessage.data[0] | inMessage.data[1] << 8); //First two bytes
    FR_rpm = (inMessage.data[2] | inMessage.data[3] << 8); //Second two bytes

    //Take average of front two, div by 60 to convert denom --> seconds, multiply by circumference in M,
    //and convert m/s to mph
    SpeedMPH = ((((FL_rpm+FR_rpm)/2.0f)/60.0f) * WHEEL_CIRCUMFERENCE_M * METERS_SEC_TO_MPH);
    Serial.println ("Receive 4") ;
}

static void receive5 (const CANMessage & inMessage) {
  Serial.println ("Receive 5") ;
}

void setup() {
  //Serial.begin(9600);
  SPI.begin();
  ACAN2515Settings settings (QUARTZ_FREQUENCY, 500UL * 1000UL) ; // CAN bit rate 500 kb/s
  const ACAN2515Mask rxm0 = extended2515Mask (0x1FFFFFFF) ;
  const ACAN2515Mask rxm1 = standard2515Mask ((RPM_ADDR | LV_ADDR | HV_ADDR | BAT_TEMP_ADDR | WSS_ADDR), 0, 0) ;
  const ACAN2515AcceptanceFilter filters [] = {
  {standard2515Filter (RPM_ADDR, 0, 0), rpm_rx}, // RXF0
  {standard2515Filter (WSS_ADDR, 0, 0), wss_rx},  // RXF1
  {standard2515Filter (HV_ADDR, 0, 0), hv_rx}, // RXF2
  {standard2515Filter (BAT_TEMP_ADDR, 0, 0), temp_rx}, // RXF3
  {standard2515Filter (LV_ADDR, 0, 0), lv_rx} // RXF4
  } ;
  //const uint16_t errorCode = can.setFiltersOnTheFly (rxm0, rxm1, filters, 5) ;
  const uint16_t errorCode = can.begin (settings, [] { can.isr () ; },
                                        rxm0, rxm1, filters, 5) ;

  //CAN debug
  /*
  if (errorCode == 0) {
    Serial.print ("Bit Rate prescaler: ") ;
    Serial.println (settings.mBitRatePrescaler) ;
    Serial.print ("Propagation Segment: ") ;
    Serial.println (settings.mPropagationSegment) ;
    Serial.print ("Phase segment 1: ") ;
    Serial.println (settings.mPhaseSegment1) ;
    Serial.print ("Phase segment 2: ") ;
    Serial.println (settings.mPhaseSegment2) ;
    Serial.print ("SJW: ") ;
    Serial.println (settings.mSJW) ;
    Serial.print ("Triple Sampling: ") ;
    Serial.println (settings.mTripleSampling ? "yes" : "no") ;
    Serial.print ("Actual bit rate: ") ;
    Serial.print (settings.actualBitRate ()) ;
    Serial.println (" bit/s") ;
    Serial.print ("Exact bit rate ? ") ;
    Serial.println (settings.exactBitRate () ? "yes" : "no") ;
    Serial.print ("Sample point: ") ;
    Serial.print (settings.samplePointFromBitStart ()) ;
    Serial.println ("%") ;
  } else {
    Serial.print ("Configuration error 0x") ;
    Serial.println (errorCode, HEX) ;
  }
  */
  
  

  LCD__init();        // Serial.begin(9600) is included in this function;
  //CAN_initialize();   
  buttons__init();    //We won't be using the buttons for now

  // initialize the first filter to search for RPM
  //CAN.init_Filt(0, 0, RPM_ADDR);

  LCD__wake();
  LCD__default();

  indicator__wake();
}

static uint32_t prev_millis = 0;
static uint32_t gUpdate = 0 ;
static uint32_t gReceivedFrameCount = 0 ;
static uint32_t gSentFrameCount = 0 ;
static uint8_t gTransmitBufferIndex = 0 ;

void loop() {
  can.dispatchReceivedMessage();
  CANMessage frame;

  if (can.available()) {
    can.receive(frame);
    /*
    if (frame.id == RPM_ADDR) {
      RPM = -(frame.data[2] | (frame.data[3] << 8));
    }
    if (frame.id == LV_ADDR) {
      LV = ((frame.data[1] | (frame.data[0] << 8)) * 0.01f) + LV_OFFSET;
    }
    if (frame.id == HV_ADDR) {
      HV = ((frame.data[0] | (frame.data[1] << 8))*0.1f);
    }
    if (frame.id == BAT_TEMP_ADDR) {
      TEMP = (frame.data[0] | (frame.data[1] << 8) | (frame.data[2] << 16) | (frame.data[3] << 32)) * 0.1f);
    }
    if (frame.id == WSS_ADDR) {
      uint16_t FL_rpm, FR_rpm;
      FL_rpm = (frame.data[0] | frame.data[1] << 8); //First two bytes
      FR_rpm = (frame.data[2] | frame.data[3] << 8); //Second two bytes

      //Take average of front two, div by 60 to convert denom --> seconds, multiply by circumference in M,
      //and convert m/s to mph
      SpeedMPH = ((((FL_rpm+FR_rpm)/2.0f)/60.0f) * WHEEL_CIRCUMFERENCE_M * METERS_SEC_TO_MPH);
    }
    */
  }
  
  if (gUpdate < millis()) {
    gUpdate += 400;
    indicator__update(RPM, SpeedMPH, TEMP);
    LCD__update(SpeedMPH, TEMP, LV, HV);
  }


  buttons__poll();
  buttons__update_LCD(&can);
  // Print the RPM, SpeedMPH, and TEMP
  // use for testing, leave commented out during real use
  // CAN__print_recieved_values(RPM, SpeedMPH, TEMP);
}
