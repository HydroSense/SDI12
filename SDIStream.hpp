#ifndef __SDI_STREAM_HPP
#define __SDI_STREAM_HPP

#include <Arduino.h>
#include <Stream.h>

class SDIStream {
public:
  virtual void sendPreamble() = 0;
  virtual void setBufferRead() = 0;
  virtual void setBufferWrite() = 0;

  virtual int available() = 0;
  virtual int read() = 0;   /* TODO(colin): change to char */
  virtual int peek() = 0;
  virtual void flush() = 0;

  virtual int write(char c) = 0;
  virtual int write(char* str) = 0;
};

#endif
