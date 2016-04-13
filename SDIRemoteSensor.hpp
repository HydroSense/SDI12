#ifndef SDI_REMOTE_SENSOR_H
#define SDI_REMOTE_SENSOR_H

#define SDI_MAX_DATA 8

#include <Arduino.h>
#include "SDISerial.hpp"

struct SDIDeviceIdentification{
  char addr[2] = {0};
  char sdiVersion[3] = {0};
  char vendor[9] = {0};
  char modelNum[7] = {0};

  char sensorVersion[4] = {0};
  char optional[14] = {0};
};

class SDIRemoteSensor {
private:
  SDIStream &mySDIStream;
  char address;
  bool busy;
  unsigned long timeReady;
  float data[SDI_MAX_DATA];
  struct SDIDeviceIdentification mySDIDeviceIdentification;
public:
  SDIRemoteSensor(SDIStream &bus, char addr);
  int listen();
  int setIdentification(SDIDeviceIdentification &id);
  struct SDIDeviceIdentification getMySDIDeviceIdentification();

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
