
//Import required libraries, including built-in SPI and Servo libraries
#include <SPI.h>
#include "RF24.h"
#include <Servo.h>

//Attachment to SPI bus is implicit:
//RF24 SCK to Pin 13
//RF24 MOSI to Pin 11
//RF24 MISO to Pin 12

//RF24 VCC to 3.3 V
//RF24 gnd to gnd

//Attach the RF24 module control pins to 7 & 8
RF24 radio(7,8);


//Set an address for a communication pipe (same as transmitter)
const uint64_t pipe1 = 0xE8E8F0F0E1LL;

//Int arry to hold data values to pass along
int data[2];

//Instantiate a servo motor
Servo servo;

void setup() {
  //Declare pins 2 & 3 as outputs for H-bridge control wires
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);

  //Activate the RF24
  radio.begin();
  //Communicate on an open channel (same as transmitter)
  radio.setChannel(108);
  //Tell the RF234 we're going to be receiving on the first pipe
  radio.openReadingPipe(1,pipe1);
  //Begin receiving
  radio.startListening();   
  //Attach servo motor to pin 6
  servo.attach(6);
}

void loop() {    
  //Only enter this section of code if there is data to read                                 
  if (radio.available()) {                           
    //Receive the data from the transmitter
    radio.read(&data, sizeof(data));

    //Store the received data in the data array
    int lrRead = data[0];
    int fbRead = data[1];

    //Map the left/right (steering) value to a servo degree angle
    //This doesn't require a full range of motion, so mapped between 75 & 125
    lrRead = map(lrRead, 0, 1023, 75, 125);

    //Move the servo to the correct position
    servo.write(180 - lrRead);

    //Compare forward/backward value to midpoint (512) to determine direction
    if (fbRead < 512) {
      //Map value between 0 and 255 to set a PWM "speed"
      int potMapCW = map(fbRead, 512, 1023, 0, 255);
      //Set the speed of the motor using PWM
      analogWrite(5, potMapCW);  
      //Call function to move motor clockwise
      cw();
    }

    if (fbRead > 512) {
      //Map value between 255 and 0 to set reverse PMW "speed"
      int potMapCCW = map(fbRead, 0, 512, 255, 0);
      //Set motor speed
      analogWrite(5, potMapCCW);  
      //Call function to move motor counter-clockwise
      ccw();
    }
  }
}

//Turn motor counter-clockwise by setting one control wire low and the other high
void ccw() {
  digitalWrite(3, LOW);
  digitalWrite(2, HIGH);
}

//Turn motor clockwise by setting one control wire low and the other high
void cw() {
  digitalWrite(2, LOW);
  digitalWrite(3, HIGH);
}


