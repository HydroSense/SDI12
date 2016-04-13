#ifndef SDI_REMOTE_SENSOR_H
#define SDI_REMOTE_SENSOR_H

#define SDI_MAX_DATA 8

#include <Arduino.h>

#include "SDISerial.hpp"

class SDIRemoteSensor {
private:
  SDIStream &mySDIStream;
  char address;
  bool busy;
  unsigned long timeReady;
  float data[SDI_MAX_DATA];
public:
  SDIRemoteSensor(SDIStream &bus, char addr);
  int listen();

/*
  // status functions
  int busy();
  static char querySensorAddress();

  // modification functions
  int changeAddress(char newAddress);

  // measurement triggers
  unsigned long startMeasurement();
  unsigned long startAltMeasurement(int altNo);

  // getters
  float* getData();
  */
};


#endif
