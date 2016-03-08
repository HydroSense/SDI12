#ifndef __SDI_STREAM_HPP
#define __SDI_STREAM_HPP

#include <Arduino.h>

class SDISerial : public Stream {
public:
  virtual void begin() = 0;
  virtual void end() = 0;
  virtual void sendPreamble() = 0;
  virtual void setBufferRead() = 0;
  virtual void setBufferWrite() = 0;
};

#endif
