#ifndef __SDI_HARDWARE_SERIAL_HPP
#define __SDI_HARDWARE_SERIAL_HPP

#include <Arduino.h>
#include <Stream.h>

#include "SDIStream.hpp"

class SDISerial : public SDIStream {
private:
  Stream &mStream;
  int mSerialOutPin;
  int mFlowControlPin;
  bool isHardwareSerial;

public:
  /* Constructors for both HardwareSerial and SoftwareSerial */
  SDISerial(Stream &stream, int serialOutPin, int flowControlPin);

  /* SDIStream Implementation */
  virtual void sendPreamble();
  virtual void setBufferRead();
  virtual void setBufferWrite();

  virtual int available();
  virtual int read();
  virtual int peek();
  virtual void flush();

  virtual int write(char c);
  virtual int write(char* str);
};

#endif
