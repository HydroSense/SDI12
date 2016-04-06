#ifndef __SDI_SERIAL_CPP
#define __SDI_SERIAL_CPP

#include "SDISerial.hpp"
#include "SDIBusController.hpp"

#include <string.h>

SDISerial::SDISerial(HardwareSerial &serial, int serialOutPin, int flowControlPin):
    mSerial(serial){
    // set the pin mode of the serial output
    pinMode(flowControlPin, OUTPUT);


    mSerialOutPin = serialOutPin;
    mFlowControlPin = flowControlPin;
    isHardwareSerial = false;
  }

//
// SDISerial Implementation
//

void SDISerial::begin() {
  mSerial.begin(9600, SERIAL_7E1);
}
void SDISerial::end() {
  mSerial.end();
}

void SDISerial::sendPreamble() {
  /*
  Sends the required preamble according to the SDI-12 specification
  */

  // TODO(colin): figure out to start and stop interface

  pinMode(mSerialOutPin, OUTPUT);
  digitalWrite(mSerialOutPin, 0);
  delay(SDI_BREAK_TIME_MS);
  digitalWrite(mSerialOutPin, 1);
  delay(SDI_MARKING_TIME_MS);
}

void SDISerial::setBufferWrite(){
  digitalWrite(mFlowControlPin, 1);
}

void SDISerial::setBufferRead(){
  // LOW is READ ******
  digitalWrite(mFlowControlPin, 0);
}

int SDISerial::available() {
  return mSerial.available();
}
int SDISerial::read() {
  return mSerial.read();
}
int SDISerial::peek() {
  return mSerial.peek();
}
void SDISerial::flush() {
  mSerial.flush();
}

//
// Print implementation
//

/* This sets the buffer to write, sends the preamble, and writes to the serial
  port. It is the caller's responsibility to set the buffer back to read */
int SDISerial::write(char chr) {
  this->setBufferWrite();

  // TODO(colin): allow for hardware serial
  int out = mSerial.write((const uint8_t*)&chr, sizeof(chr));

  this->setBufferRead();

  return out;
}
int SDISerial::write(char* str) {
  this->setBufferWrite();

  // TODO(colin): allow for enabling and disabling hardware serial
  int out = mSerial.write((const uint8_t*)str, strlen(str));

  this->setBufferRead();

  return out;
}

#endif
