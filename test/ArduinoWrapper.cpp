#ifndef __ARDUINO_WRAPPER_CPP
#define __ARDUINO_WRAPPER_CPP

#include "Arduino.h"

#include <stdio.h>
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
  tim.tv_nsec = 1000L * (long)usec;
  nanosleep(&tim, NULL);
}
void delay(int msec) {
  cout << "Delay: " << msec << " ms" << endl;

  struct timespec tim;
  tim.tv_sec = 0;
  tim.tv_nsec = 1000L*1000L * (long)msec;
  nanosleep(&tim, NULL);
}

ShimSerial::ShimSerial() {
  mActive = false;
  mBufferIndex = 0;

  mType = 0;
  mBaud = 0;
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
  printf("Serial1.write: %c\n", chr);
}
void ShimSerial::write(char* str) {
  printf("Serial1.write: %s\n", str);
}

int ShimSerial::available() {
  return mBufferIndex > 0;
}
char ShimSerial::read() {
  if (this->available()) {
    return mBuffer[mBufferIndex--];
  } else {
    cout << "ERR: Buffer Overflow!\n" << endl;
    return '\0';
  }
}



#endif
