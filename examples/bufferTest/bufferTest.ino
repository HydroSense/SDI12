/*
   Sanity check for the tri-state buffer.
*/

//#include "SDI.h"

#define SERIAL_OUTPUT_PIN 1
#define FLOW_CONTROL_PIN A3

//SDIBusController *SDIBus;

void setup(){
/*
    // instantiate SDISerial instance with hardware Serial1
    SDISerial *mySDISerial = new SDISerial(Serial1, SERIAL_OUTPUT_PIN, FLOW_CONTROL_PIN);

    // instantiate SDIBus controller, passing in hardware Serial1 as argument
    SDIBus = new SDIBusController(*mySDISerial);

*/

    pinMode(FLOW_CONTROL_PIN, OUTPUT);

    // Start HardwareSerial
    Serial1.begin(1200);

    // For debugging to the computer
    Serial.begin(9600);

}

void loop(){
    pinMode(FLOW_CONTROL_PIN, OUTPUT);
    digitalWrite(FLOW_CONTROL_PIN, LOW); // write mode
    delay(50);
    Serial1.write('0');
    delay(50);
    digitalWrite(FLOW_CONTROL_PIN, HIGH); // read mode???????????fuck

    // The following should NOT appear, because the buffer is set to READ:
    Serial1.write('1');

    delay(1000);
}

void setBufferWrite(){
    digitalWrite(FLOW_CONTROL_PIN, 0);
}

void setBufferRead(){
    digitalWrite(FLOW_CONTROL_PIN, 1);
}
