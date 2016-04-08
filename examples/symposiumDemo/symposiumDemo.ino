/*
   Example sketch sending a stream of data to a python script to graph.
   Colby Rome 4-6-16

*/

#include "SDI.h"
#include <Wire.h>

#define SERIAL_OUTPUT_PIN 1
#define FLOW_CONTROL_PIN A3

SDIBusController *SDIBus;
char addr;

void setup(){
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);

  Wire.begin();

  Wire.beginTransmission(0b1110000);
  Wire.write(byte(0x03));
  Wire.write(0b00000000);  //Sets all pins to output
  Wire.endTransmission();

  Wire.beginTransmission(0b1110000);
  Wire.write(byte(0x01));
  Wire.write(0b11111111);  //Sets appropriate pins to high/low
  Wire.endTransmission();


    // instantiate SDISerial instance with hardware Serial1
    pinMode(FLOW_CONTROL_PIN, OUTPUT);
    //pinMode(FLOW_CONTROL_PIN, OUTPUT);
    SDISerial *mySDISerial = new SDISerial(Serial1, SERIAL_OUTPUT_PIN, FLOW_CONTROL_PIN);

    // instantiate SDIBus controller, passing in hardware Serial1 as argument
    SDIBus = new SDIBusController(*mySDISerial);

    // Address of Decagon CTD
    addr = '0';

    // For debugging to the computer
    Serial.begin(9600);


}

void loop(){
    int altno = -1; // 'regular' refreshh function

    // The following will be populated by the call to refresh
    int waitTime;
    int numExpected;

    int res = SDIBus->refresh(addr, altno, &waitTime, &numExpected);
    if(res != 0){
      Serial.print("Received res = ");
      Serial.println(res);
    }
    else{
        delay(1000);
        float buffer[numExpected];
        res = SDIBus->getData(addr, buffer, numExpected);
        if(res != 0){
          Serial.print("Error during getData. Received: ");
          Serial.println(res);
        }
        else{
          for(int i=0; i<numExpected; i++){
            if(buffer[i] >= 0){
              Serial.print('+');
            }
            Serial.print(buffer[i]);
          }
          Serial.print("\r\n");

        }
    }
    delay(1000);
}
