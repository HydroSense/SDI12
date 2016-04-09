#ifndef __SDI_HARDWARE_SERIAL_HPP
#define __SDI_HARDWARE_SERIAL_HPP

#include <Arduino.h>
#include <Stream.h>

#include "SDIStream.hpp"
#define SERIAL_7E1 0x24

class SDISerial : public SDIStream {
private:
  HardwareSerial &mSerial;
  int mSerialOutPin;
  int mFlowControlPin;
  bool isHardwareSerial;

public:
  /* Constructors for both HardwareSerial and SoftwareSerial */
  SDISerial(HardwareSerial &stream, int serialOutPin, int flowControlPin);

  /* SDIStream Implementation */
  virtual void begin();
  virtual void end();

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
