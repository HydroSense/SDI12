#ifndef __HARDWARE_SERIAL_CPP
#define __HARDWARE_SERIAL_CPP

// We will use this as simulating HardwareSerial

#include <string>
#include "FakeSerial.hpp" // needed because Stream is defined there
using namespace std;


class HardwareSerial : public Stream {
private:
  bool mActive;
  bool mUsingDomainSocket;
  int mListenSocket;
  int mDomainSocket;

  SerialType mSerialType;
  int mBaud;

  string mSerialInputBuffer;
  string mSerialOutputHistory;

public:
  HardwareSerial();

  // extra stuff not compatible with Arduino environment
  //void serveDomainSocket();
  //int connectDomainSocket();
  //int disconnectDomainSocket();

public:
  void addToInputBuffer(const char* s);
  void addToInputBuffer(const string& s);
  void clearInputBuffer();
  const string& getOutputHistory();
  void clearOutputHistory();
  SerialType getSerialType();
  int getBaudRate();


  // Arduino compatable API
  // Stream implementation
  virtual void begin(int baud, SerialType type);
  virtual void end();

  virtual void write(char chr);
  virtual void write(char* str);

  virtual int active();
  virtual int available();
  virtual int read();
};

extern HardwareSerial Serial2;

#endif
