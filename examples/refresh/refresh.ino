/*
   Example sketch for testing the refresh and getData functions
   Colby Rome 4-4-16
*/

#include "SDI.h"

#define SERIAL_OUTPUT_PIN 1
#define FLOW_CONTROL_PIN A3

SDIBusController *SDIBus;
char addr;

void setup(){
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
        Serial.print("Error: didn't respond: received ");
        Serial.println(res);
    }
    else{
        Serial.print("Success. read waitTime:");
        Serial.println(waitTime);
        Serial.print("read numExpected: ");
        Serial.println(numExpected);
        Serial.flush();
        delay(1000);
        float buffer[3];
        res = SDIBus->getData(addr, buffer, numExpected);
        if(res != 0){
          Serial.print("Error during getData. Received: ");
          Serial.println(res);
        }
        else{
          Serial.println("Success. Read: ");
          Serial.println(buffer[0]);
          Serial.println(buffer[1]);
          Serial.println(buffer[2]);
        }

    }

    delay(1000);
}
