#ifndef __SDI_SERIAL_CPP
#define __SDI_SERIAL_CPP

#include "SDISerial.hpp"
#include "SDIBusController.hpp"

#include <iostream>
using namespace std;

SDISerial::SDISerial(FakeSerial &stream, int serialOutPin, int flowControlPin):
  mStream(stream){
    mSerialOutPin = serialOutPin;
    mFlowControlPin = flowControlPin;
    isHardwareSerial = false;
  }

//
// SDISerial Implementation
//

void SDISerial::begin() {
  // Begin with configuration 1200 baud, SERIAL_7E1
  if(isHardwareSerial){
    /*
    // Testing typecasting:
    HardwareSerial &newStream = dynamic_cast<HardwareSerial &>(mStream);
    newStream.begin(1200, SERIAL_7E1);
    //((HardwareSerial) mStream).begin(1200, SERIAL_7E1);
    */
  }
  else{
//    FakeSerial &newStream = static_cast<FakeSerial &>(mStream);
    FakeSerial &newStream = static_cast<FakeSerial &>(mStream);
    newStream.begin(1200, SERIAL_7E1);
    //((SoftwareSerial) mStream).begin(1200, SERIAL_7E1);
  }
}

void SDISerial::end() {
  if(isHardwareSerial){
    /*
    HardwareSerial &newStream = dynamic_cast<HardwareSerial &>(mStream);
    newStream.end();
    //((HardwareSerial)mStream).end();
    */
  }
  else{
    FakeSerial &newStream = static_cast<FakeSerial &>(mStream);
    newStream.end();
    //((SoftwareSerial)mStream).end();
  }
}

void SDISerial::sendPreamble() {
  /*
  Sends the required preamble according to the SDI-12 specification
  */

    // Stop mSerial
    this->end();

    pinMode(mSerialOutPin, OUTPUT);
    digitalWrite(mSerialOutPin, 1);
    delay(SDI_BREAK_TIME_MS);
    digitalWrite(mSerialOutPin, 0);
    delay(SDI_MARKING_TIME_MS);

    // re-enable mSerial
    this->begin();
}

void SDISerial::setBufferWrite(){
    digitalWrite(mFlowControlPin, 0);
}

void SDISerial::setBufferRead(){
    digitalWrite(mFlowControlPin, 1);
}

//
// Stream Implementation
//

int SDISerial::available() {
  return mStream.available();
}
int SDISerial::read() {
  return mStream.read();
}
int SDISerial::peek() {
  return mStream.peek();
}
void SDISerial::flush() {
  mStream.flush();
}

//
// Print implementation
//

/* This sets the buffer to write, sends the preamble, and writes to the serial
  port. It is the caller's responsibility to set the buffer back to read */
size_t SDISerial::write(char chr) {
  this->setBufferWrite();
  //return mStream.write(val);
}
size_t SDISerial::write(char* str) {
  this->setBufferWrite();
  //return mStream.write(str, size);
}

#endif
