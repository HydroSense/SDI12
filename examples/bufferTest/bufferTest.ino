/*
   Sanity check for the tri-state buffer.
   Colby Rome 3-30-16
*/

//#include "SDI.h"

#define SERIAL_OUTPUT_PIN 1
#define FLOW_CONTROL_PIN A3

#define USEC_PER_BYTE 8333

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
      digitalWrite(FLOW_CONTROL_PIN, LOW); // LOW -> read mode
    Serial1.write('1'); // write is non-blocking. Sends data over ISR
    Serial1.flush();

    // Wait until we've written everything.

    // Back to read mode
    digitalWrite(FLOW_CONTROL_PIN, HIGH); // HIGH -> write mode

    // The following should NOT appear, because the buffer is set to READ:
    Serial1.write('1');

    delay(100);
}

void setBufferWrite(){
    digitalWrite(FLOW_CONTROL_PIN, LOW);
}

void setBufferRead(){
    digitalWrite(FLOW_CONTROL_PIN, HIGH);
}
