#ifndef __SDI_SERIAL_CPP
#define __SDI_SERIAL_CPP

#include "SDISerial.hpp"

SDISerial::SDISerial(Stream& stream, int serialOutPin, int digitalWritePin):
  mStream(stream){
    mSerialOutPin = serialOutPin;
    mDigitalWritePin = digitalWritePin;
}

//
// SDISerial Implementation
//

void SDISerial::begin() {

}
void SDISerial::end() {

}
void SDISerial::sendPreamble() {

}
void SDISerial::setBufferRead() {

}
void SDISerial::setBufferWrite() {

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
