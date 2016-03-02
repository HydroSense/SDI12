#ifndef __ARDUINO_WRAPPER_CPP
#define __ARDUINO_WRAPPER_CPP

#include "Arduino.h"

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>

#include <stdlib.h>
#include <time.h>
#include <iostream>
using namespace std;

// define the shim interface
ShimSerial Serial1;

void pinMode(int pin, int isInput) {
  if (isInput) {
    cout << "Pin: " << pin << " configured for INPUT" << endl;
  } else {
    cout << "Pin: " << pin << " configured for OUTPUT" << endl;
  }
}

void digitalWrite(int pin, int isHigh) {
  if (isHigh) {
    cout << "Pin: " << pin << " is now HI" << endl;
  } else {
    cout << "Pin: " << pin << " is now LO" << endl;
  }
}

void delayMicroseconds(int usec) {
  cout << "Delay: " << usec << " usec" << endl;

  struct timespec tim;
  tim.tv_sec = 0;
  tim.tv_nsec = 100L*1000L * (long)usec;
  nanosleep(&tim, NULL);
}

void delay(int msec) {
  cout << "Delay: " << msec << " ms" << endl;

  struct timespec tim;
  tim.tv_sec = 0;
  tim.tv_nsec = 100L*1000L*1000L * (long)msec;
  nanosleep(&tim, NULL);
}

unsigned long millis() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return tp.tv_sec * 1000 + tp.tv_usec / 1000;
}

ShimSerial::ShimSerial() {
  mListenSocket = -1;
  mDomainSocket = -1;

  mActive = false;

  mType = 0;
  mBaud = 0;
}

void ShimSerial::serveDomainSocket() {
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

void ShimSerial::connectDomainSocket() {
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
    exit(-1);
  }
}

void ShimSerial::disconnectDomainSocket() {
  close(mDomainSocket);
  if (mListenSocket > -1) {
    close(mListenSocket);
  }
}

void ShimSerial::begin(int baud, int type) {
  mBaud = baud;
  mType = type;

  cout << "Started Serial" << endl;
  mActive = true;
}
void ShimSerial::end() {
  cout << "Ended Serial" << endl;
  mActive = false;
}

void ShimSerial::write(char chr) {
  cout << "Serial1.write: " << chr << endl;

  send(mDomainSocket, &chr, sizeof(char), 0);
}
void ShimSerial::write(char* str) {
  cout << "Serial1.write: " << str << endl;

  send(mDomainSocket, str, strlen(str)*sizeof(char), 0);
}

int ShimSerial::available() {
  int count;
  int res = ioctl(mDomainSocket, FIONREAD, &count);
  if (res < 0) {
    perror("IOCTL Error: ");
    exit(-1);
  }

//  return count > 0;
  return count;
}
char ShimSerial::read() {
  char out;
  if (this->available()) {
    recv(mDomainSocket, &out, sizeof(char), 0);
    cout << "Serial1.read: " << out << endl;
    return out;
  } else {
    cout << "Serial1.read: (NO DATA)" << endl;
    return '\255';
  }
}



#endif
