#ifndef __FAKE_SERIAL_CPP
#define __FAKE_SERIAL_CPP

#include <string>
using namespace std;

enum SerialType {
  SERIAL_7E1,
  UNDEF
};

class Stream {
public:
  virtual void begin(int baud, SerialType type) = 0;
  virtual void end(void) = 0;

  virtual void write(char chr) = 0;
  virtual void write(char* str) = 0;

  virtual int active() = 0;
  virtual int available() = 0;

  virtual char read() = 0;
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
  // Stream implementation
  virtual void begin(int baud, SerialType type);
  virtual void end();

  virtual void write(char chr);
  virtual void write(char* str);

  virtual int active();
  virtual int available();
  virtual char read();
};

extern FakeSerial Serial1;

#endif
