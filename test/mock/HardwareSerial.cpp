#include "HardwareSerial.hpp"

#include <errno.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#include <iostream>
using namespace std;

HardwareSerial Serial2;

HardwareSerial::HardwareSerial() {
  mActive = false;
  mUsingDomainSocket;
  mListenSocket = -1;
  mDomainSocket = -1;

  mSerialType = UNDEF;
  mBaud = 0;

  mSerialInputBuffer = "";
  mSerialOutputHistory = "";
}

/*
void HardwareSerial::serveDomainSocket() {
  int res;

  mListenSocket = socket(AF_UNIX, SOCK_STREAM, 0);
  if (mListenSocket < 0) {
    perror("Socket Error: ");
    exit(-1);
  }

  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, "SDI12.socket", sizeof(addr.sun_path)-1);
  unlink("SDI12.socket");

  bind(mListenSocket, (struct sockaddr*)&addr, sizeof(addr));
  if (mListenSocket < 0) {
    perror("Bind Error: ");
    exit(-1);
  }

  res = listen(mListenSocket, 1);
  if (res < 0) {
    perror("Listen Error: ");
    exit(-1);
  }

  mDomainSocket = accept(mListenSocket, NULL, NULL);
  if (mDomainSocket < 0) {
    perror("Accept Error: ");
    exit(-1);
  }
}

int HardwareSerial::connectDomainSocket() {
  int res;

  mDomainSocket = socket(AF_UNIX, SOCK_STREAM, 0);
  if (mDomainSocket < 0) {
    perror("Socket Error: ");
    exit(-1);
  }

  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, "SDI12.socket", sizeof(addr.sun_path)-1);

  res = connect(mDomainSocket, (struct sockaddr*)&addr, sizeof(addr));
  if (res < 0) {
    perror("Connect Error: ");
    return -1;
  }

  return 0;
}

int HardwareSerial::disconnectDomainSocket() {
  close(mDomainSocket);
  if (mListenSocket > -1) {
    close(mListenSocket);
  }

  return 0;
}
*/

void HardwareSerial::addToInputBuffer(const char* s) {
  mSerialInputBuffer += s;
}

void HardwareSerial::addToInputBuffer(const string& s) {
  mSerialInputBuffer += s;
}

void HardwareSerial::clearInputBuffer() {
  mSerialInputBuffer = "";
}

const string& HardwareSerial::getOutputHistory() {
  return (const string&)mSerialOutputHistory;
}

void HardwareSerial::clearOutputHistory() {
  mSerialOutputHistory = "";
}

SerialType HardwareSerial::getSerialType() {
  return mSerialType;
}

int HardwareSerial::getBaudRate() {
  return mBaud;
}


void HardwareSerial::begin(int baud, SerialType type) {
  mBaud = baud;
  mSerialType = type;

  mActive = true;
}

void HardwareSerial::end() {
  mActive = false;
}

void HardwareSerial::write(char chr) {
  // append to the output history
  mSerialOutputHistory += chr;

  if (mUsingDomainSocket) {
    send(mDomainSocket, &chr, sizeof(char), 0);
  }
}
void HardwareSerial::write(char* str) {
  // append to the output history
  mSerialOutputHistory += str;

  if (mUsingDomainSocket) {
    send(mDomainSocket, str, strlen(str)*sizeof(char), 0);
  }
}

int HardwareSerial::active() {
  return (int)mActive;
}

// TODO(colin): add support for domain socket
int HardwareSerial::available() {
  return mSerialInputBuffer.length();

  /*
  int count;
  int res = ioctl(mDomainSocket, FIONREAD, &count);
  if (res < 0) {
    perror("IOCTL Error: ");
    exit(-1);
  }

  return count > 0;
  */
}

// TODO(colin): add support for domain socket
// TODO: make sure return type int is correct..
int HardwareSerial::read() {
  if (!this->available()) {
    return '\0';
  }

  // get the first element out and then
  char c = mSerialInputBuffer[0];
  mSerialInputBuffer.erase(0, 1);

  return (int)c;

  /*
  char out;
  if (this->available()) {
    recv(mDomainSocket, &out, sizeof(char), 0);
    cout << "Serial1.read: " << out << endl;
    return out;
  } else {
    cout << "Serial1.read: (NO DATA)" << endl;
    return '\255';
  }
  */
}
int HardwareSerial::peek(){
  if(!this->available()){
    return '\0';
  }
  return (int) mSerialInputBuffer[0];
}

bool HardwareSerial::flush(){
  return true; // TODO implement
}
