#include "Arduino.h"

#ifndef __ARDUINO_WRAPPER_CPP
#define __ARDUINO_WRAPPER_CPP

#include "Arduino.h"

#include <time.h>

#include <iostream>
using namespace std;

ArduinoEnvironment Environment;

ArduinoEnvironment::ArduinoEnvironment() {
  this->reset();
}

void ArduinoEnvironment::reset() {
  timeUsec = 0;

  for (int i = 0; i < ARDUINO_NUM_PINS; i++) {
    pinTristates[i] = PIN_STATE_UNDEF;
    pinSignalStates[i] = SIGNAL_STATE_UNDEF;
  }
}

void pinMode(int pin, int isInput) {
  if (isInput) {
    Environment.pinTristates[pin] = IN;
  } else {
    Environment.pinTristates[pin] = OUT;
  }
}

void digitalWrite(int pin, int isHigh) {
  // ensure that the tristate is correct
  if (Environment.pinTristates[pin] != OUT) {
    throw PinModeException();
  }

  if (isHigh) {
    Environment.pinSignalStates[pin] = HI;
  } else {
    Environment.pinSignalStates[pin] = LO;
  }
}

void delayMicroseconds(long usec) {
  cout << "Delay: " << usec << " usec" << endl;

  // spinlock until the time is up
  struct timespec startTime, currentTime;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &startTime);
  long timeDiff = 0;
  while(timeDiff < usec) {
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &currentTime);
    timeDiff = currentTime.tv_nsec/1000 - startTime.tv_nsec/1000;
  }

  // increment the current time
  Environment.timeUsec += timeDiff;
}

void delay(int msec) {
  delayMicroseconds(1000*msec);
}




#endif
