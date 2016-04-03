#ifndef SDI_REMOTE_SENSOR_H
#define SDI_REMOTE_SENSOR_H

#define SDI_MAX_DATA 8

#include "SDIBusController.hpp"

class SDIRemoteSensor {
private:
  SDIBusController& mBus;
  char mAddress;
  bool mBusy;
  unsigned long mTimeReady;
  float mData[SDI_MAX_DATA];
public:
  SDIRemoteSensor(SDIBusController& bus, char address);

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
}

#endif
