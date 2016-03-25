#ifndef __FAKE_SERIAL_CPP
#define __FAKE_SERIAL_CPP

// We will use this as simulating SoftwareSerial

#include <string>
#include "Stream.hpp"
using namespace std;

enum SerialType {
  SERIAL_7E1,
  UNDEF
};

class FakeSerial : public Stream {
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
  FakeSerial();

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
   void begin(int baud, SerialType type);
   void end();
   int active();

  // Stream implementation
   int available();
   int read();
   int peek();
   void flush();

   size_t write(char chr);
   size_t write(char* str);

};

extern FakeSerial Serial1;

#endif
