#ifndef __SDI_HARDWARE_SERIAL_HPP
#define __SDI_HARDWARE_SERIAL_HPP

#include <Arduino.h>

#include "SDIStream.hpp"

class SDISerial : public SDIStream {
private:
  Stream &mStream;
  int mSerialOutPin;
  int mFlowControlPin;
  bool isHardwareSerial;

public:
  /* Constructors for both HardwareSerial and SoftwareSerial */
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
  size_t write(uint8_t);
  size_t write(char* str);
};

#endif
