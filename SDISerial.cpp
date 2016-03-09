#ifndef __SDI_SERIAL_CPP
#define __SDI_SERIAL_CPP

#include "SDISerial.hpp"

SDISerial::SDISerial(SoftwareSerial &stream, int serialOutPin, int flowControlPin):
  mStream(stream){
    mSerialOutPin = serialOutPin;
    mDigitalWritePin = flowControlPin;
    isHardwareSerial = false;
  }

SDISerial::SDISerial(HardwareSerial &stream, int serialOutPin, int flowControlPin):
  mStream(stream){
    mSerialOutPin = serialOutPin;
    mDigitalWritePin = flowControlPin;
    isHardwareSerial = true;
}

//
// SDISerial Implementation
//

void SDISerial::begin() {
  // Begin with configuration 1200 baud, SERIAL_7E1
  if(isHardwareSerial){
    ((HardwareSerial)mStream).begin(1200, SERIAL_7E1);
  }
  else{
    ((SoftwareSerial)mStream).begin(1200, SERIAL_7E1);
  }
}

void SDISerial::end() {
  if(isHardwareSerial){
    ((HardwareSerial)mStream).end();
  }
  else{
    ((SoftwareSerial)mStream).end();
  }
}

void SDIBusController::sendPreamble() {
  /*
  Sends the required preamble according to the SDI-12 specification
  */

    // Stop mSerial
    this.end();

    pinMode(mSerialOutputPin, OUTPUT);
    digitalWrite(mSerialOutputPin, 1);
    delay(SDI_BREAK_TIME_MS);
    digitalWrite(mSerialOutputPin, 0);
    delay(SDI_MARKING_TIME_MS);

    // re-enable mSerial
    this.begin(1200, SERIAL_7E1);
}

void SDIBusController::setBufferWrite(){
    digitalWrite(mFlowControlPin, 0);
}

void SDIBusController::setBufferRead(){
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
  return mStream.flush();
}

//
// Print implementation
//

size_t SDISerial::write(uint8_t val) {
  // TODO(colby,matt): figure out which direction
  digitalWrite(mFlowControlPin, ____);
  return mSerial.write(val);
}
size_t SDISerial::write(const char* str, size_t size) {
  // TODO(colby,matt): figure out which direction
  digitalWrite(mFlowControlPin, ____);
  return mSerial.write(str, size);
}

#endif
