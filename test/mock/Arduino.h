#ifndef __ARDUINO_H
#define __ARDUINO_H

#define INPUT 1
#define OUTPUT 0

#define SERIAL_7E1 0

void pinMode(int pin, int isInput);
void digitalWrite(int pin, int isHigh);

void delayMicroseconds(int usec);
void delay(int msec);
unsigned long millis();

#define SHIM_SERIAL_BUFFER_SIZE 4096
class ShimSerial {
private:
  bool mActive;
  int mListenSocket;
  int mDomainSocket;

  int mType;
  int mBaud;

public:
  ShimSerial();
  void serveDomainSocket();
  int connectDomainSocket();
  int disconnectDomainSocket();

  void begin(int baud, int type);
  void end();

  void write(char chr);
  void write(char* str);

  int available();
  char read();

};

extern ShimSerial Serial1;

#endif
