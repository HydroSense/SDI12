#ifndef __SDI_HARDWARE_SERIAL_HPP
#define __SDI_HARDWARE_SERIAL_HPP

#include <Arduino.h>

#include "SDIStream.hpp"

#define SoftwareSerial FakeSerial //??????

class SDISerial : public SDIStream {
private:
  Stream &mStream;
  int mSerialOutPin;
  int mFlowControlPin;
  bool isHardwareSerial;

public:
  /* Constructors for both HardwareSerial and SoftwareSerial */
  SDISerial(SoftwareSerial &stream, int serialOutPin, int flowControlPin):
  SDISerial(HardwareSerial &stream, int serialOutPin, int flowControlPin);

  /* SDISerial Implementation */
  void begin();
  void end();
  void sendPreamble();
  void setBufferRead();
  void setBufferWrite();

  /* Stream Implementation */
  int available();
  int read();
  int peek();
  void flush();

  /* Print Implementation */
  size_t write(uint8_t val);
  size_t write(const char* str, size_t size);
};

#endif
