#ifndef __SDI_REMOTE_SENSOR_CPP
#define __SDI_REMOTE_SENSOR_CPP


#include <cmath>
#include <Arduino.h>

#include "SDIRemoteSensor.hpp"
#include "SDIBusController.hpp"

/* Public Members */

SDIRemoteSensor::SDIRemoteSensor(char addr) {
  mAddr = addr;
}

int SDIRemoteSensor::changeAddress(char newAddr) {
  // change the address before setting the internal variable
  int status = SDIBus.changeAddress(mAddr, newAddr);
  if (status < 0) {
    return -1;
  }

  mAddr = newAddr;
}

char SDIRemoteSensor::getAddress() {
  return mAddr;
}

int SDIRemoteSensor::available() {
  return mAvailable;
}

int SDIRemoteSensor::refresh() {
  return this->refresh(0);
}
int SDIRemoteSensor::refresh(int altno) {
  int timeToWait = SDIBus.refresh(mAddr, altno);
  if (timeToWait < 0) {
    return timeToWait;
  }

  mReadyTimestamp = 1000*timeToWait + millis();
  mBusy = 1;
//  return status;
    return 0; //?
}

const float* SDIRemoteSensor::values() {
  mAvailable = 0;
  return (const float*)mValues;
}

const float SDIRemoteSensor::value(int index) {
  if (index > 9) {
    return nan("");
  }

  mAvailable = 0;
  return mValues[index];
}

#endif
