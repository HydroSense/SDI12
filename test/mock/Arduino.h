#ifndef __ARDUINO_H
#define __ARDUINO_H

// pull in all other mock dependencies
#include "FakeSerial.hpp"

#include <exception>
using namespace std;

#define INPUT 1
#define OUTPUT 0

#define ARDUINO_NUM_PINS 32

enum PinState {
  IN=1,
  OUT=0,
  PIN_STATE_UNDEF
};
enum SignalState {
  HI=1,
  LO=0,
  SIGNAL_STATE_UNDEF
};

// for invalid pin modes
class PinModeException : public exception {
  virtual const char* what() const throw() {
    return "ERR: Invalid pin mode";
  }
};

void pinMode(int pin, int isInput);
void digitalWrite(int pin, int isHigh);

void delayMicroseconds(long usec);
void delay(int msec);
unsigned long millis();

struct ArduinoEnvironment {
public:

  long timeUsec;
  PinState pinTristates[ARDUINO_NUM_PINS];
  SignalState pinSignalStates[ARDUINO_NUM_PINS];

  ArduinoEnvironment();

  void reset();
};

extern ArduinoEnvironment Environment;



#endif
