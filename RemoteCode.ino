
//Import required libraries, including built-in SPI
#include <SPI.h>
#include "RF24.h"


//Attachment to SPI bus is implicit:
//RF24 SCK to Pin 13
//RF24 MOSI to Pin 11
//RF24 MISO to Pin 12

//RF24 VCC to 3.3 V
//RF24 gnd to gnd

//Attach the RF24 module control pins to 9 & 10
RF24 radio(9,10);


//Set an address for a communication pipe (same as receiver)
const uint64_t pipe1 = 0xE8E8F0F0E1LL;

//Int array to hold data values to pass along
int data[2];

void setup() {
  //Activate the RF24
  radio.begin();
  //Communicate on an open channel (same as receiver)
  radio.setChannel(108); 
  //Tell the RF24 we're going to be transmitting
  radio.openWritingPipe(pipe1); 
  
}

void loop() {

  //Read in voltages from joysticks 
  int lrRead = analogRead(0); //VRX on joystick
  int fbRead = analogRead(1); //VRY on joystick

  //Store joystick values in data array
  data[0] = lrRead;
  data[1] = fbRead;

  //Transmit data array
  radio.write(&data, sizeof(data));
  
} 

