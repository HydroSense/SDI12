/*
   Example sketch for testing a sensor is active.
   Colby Rome 3-30-16
*/

#include "SDI.h"
#include <Wire.h>

#define SERIAL_OUTPUT_PIN 1
#define FLOW_CONTROL_PIN A3

SDIBusController *SDIBus;

void powerSDIMiddlePort(){
    // Powers the middle port on the Hydrosense Datalogger 2.1.2
    pinMode(5, OUTPUT);
    digitalWrite(5, HIGH);

    Wire.begin();

    Wire.beginTransmission(0b1110000);
    Wire.write(byte(0x03));
    Wire.write(0b00000000);  //Sets all pins to output
    Wire.endTransmission();

    Wire.beginTransmission(0b1110000);
    Wire.write(byte(0x01));
    Wire.write(0b00000100);  //Sets only Port2On. This is either mislabeled
    // on the PDF, or incorrectly routed. Pin P2 is on which is incorrectly
    // called Port3On
    Wire.endTransmission();
}

void setup(){
    powerSDIMiddlePort();
    // instantiate SDISerial instance with hardware Serial1
    SDISerial *mySDISerial = new SDISerial(Serial1, SERIAL_OUTPUT_PIN, FLOW_CONTROL_PIN);

    // instantiate SDIBus controller, passing in hardware Serial1 as argument
    SDIBus = new SDIBusController(*mySDISerial);

    // For debugging to the computer
    Serial.begin(9600);
}

void loop(){
    int res = SDIBus->acknowledgeActive('0');
    if(res != 0){
        Serial.println("Error: didn't respond");
    }
    else{
        Serial.println("Success.");
    }

    delay(1000);
}
