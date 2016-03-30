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
    digitalWrite(FLOW_CONTROL_PIN, LOW); // LOW -> write mode
    Serial1.write('0'); // write is non-blocking. Sends data over ISR

    // Wait until we've written everything.
    // Doesn't have an FPU. 1/1200 is 833.3 (repeating) microseconds per BIT
    // Therefore, each character is 7 bits + 1 start bit + 1 parity bit + 1 stop bit
    // 10 bits per byte sent, or 8333.3 microseconds PER BYTE
    delayMicroseconds(1 * USEC_PER_BYTE); // Delay 1 byte

    // Back to read mode
    digitalWrite(FLOW_CONTROL_PIN, HIGH); //

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
