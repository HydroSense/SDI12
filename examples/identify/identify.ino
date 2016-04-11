/*
   Example sketch for testing the identify function.
   Colby Rome 4-11-16
*/

#include "SDI.h"
#include <Wire.h>

#define SERIAL_OUTPUT_PIN 1
#define FLOW_CONTROL_PIN A3

SDIBusController *SDIBus;
char addr;

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
    pinMode(FLOW_CONTROL_PIN, OUTPUT);
    //pinMode(FLOW_CONTROL_PIN, OUTPUT);
    SDISerial *mySDISerial = new SDISerial(Serial1, SERIAL_OUTPUT_PIN, FLOW_CONTROL_PIN);

    // instantiate SDIBus controller, passing in hardware Serial1 as argument
    SDIBus = new SDIBusController(*mySDISerial);

    addr = '0'; // Decagon CTD-10 sensor

    // For debugging to the computer
    Serial.begin(9600);
}

void loop(){
    struct SDIDeviceIdentification Decagon; // declare space for result of identify

    int res = SDIBus->identify(addr, &Decagon);
    if(res != 0){
        Serial.print("Error: didn't respond: received ");
        Serial.println(res);
    }
    else{
        Serial.println("Success. Read:");

        Serial.print("addr: ");
        for(int i=0; Decagon.addr[i] != '\0'; i++){
          Serial.print(Decagon.addr[i]);
        }
        Serial.println();
        Serial.print("sdiVersion: ");
        for(int i=0; Decagon.sdiVersion[i] != '\0'; i++){
          Serial.print(Decagon.sdiVersion[i]);
        }
        Serial.println();
        Serial.print("vendor: ");
        for(int i=0; Decagon.vendor[i] != '\0'; i++){
          Serial.print(Decagon.vendor[i]);
        }
        Serial.println();
        Serial.print("modelNum: ");
        for(int i=0; Decagon.modelNum[i] != '\0'; i++){
          Serial.print(Decagon.modelNum[i]);
        }
        Serial.println();
        Serial.print("sensorVersion: ");
        for(int i=0; Decagon.sensorVersion[i] != '\0'; i++){
          Serial.print(Decagon.sensorVersion[i]);
        }
        Serial.println();
        Serial.print("optional: ");
        for(int i=0; Decagon.optional[i] != '\0'; i++){
          Serial.print(Decagon.optional[i]);
        }
        Serial.println();
    }

    delay(1000);
}
