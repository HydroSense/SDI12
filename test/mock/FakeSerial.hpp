#ifndef __FAKE_SERIAL_CPP
#define __FAKE_SERIAL_CPP

#include <string>
using namespace std;

enum SerialType {
  SERIAL_7E1,
  UNDEF
};

class FakeSerial {
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

  void write(char chr);
  void write(char* str);

  int active();
  int available();
  char read();
};

extern FakeSerial Serial1;

#endif
