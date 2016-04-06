/*
   Example sketch for testing the addressQuery function.
   Colby Rome 3-30-16
*/

#include "SDI.h"

#define SERIAL_OUTPUT_PIN 1
#define FLOW_CONTROL_PIN A3

SDIBusController *SDIBus;

void setup(){
    // instantiate SDISerial instance with hardware Serial1
    pinMode(FLOW_CONTROL_PIN, OUTPUT);
    //pinMode(FLOW_CONTROL_PIN, OUTPUT);
    SDISerial *mySDISerial = new SDISerial(Serial1, SERIAL_OUTPUT_PIN, FLOW_CONTROL_PIN);

    // instantiate SDIBus controller, passing in hardware Serial1 as argument
    SDIBus = new SDIBusController(*mySDISerial);

    // For debugging to the computer
    Serial.begin(9600);
}

void loop(){
    char readAddress;
    int res = SDIBus->addressQuery(&readAddress);
    if(res != 0){
        Serial.print("Error: didn't respond: received ");
        Serial.println(res);
    }
    else{
        Serial.print("Success. read address:");
        Serial.println(readAddress);
    }

    delay(1000);
}
