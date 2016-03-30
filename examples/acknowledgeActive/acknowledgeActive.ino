/*
   Example sketch for testing a sensor is active.
   Colby Rome 3-30-16
*/

#include "SDI.h"

#define SERIAL_OUTPUT_PIN 1
#define FLOW_CONTROL_PIN A3

SDIBusController *SDIBus;

void setup(){
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
