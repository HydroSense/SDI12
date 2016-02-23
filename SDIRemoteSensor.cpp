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

  mReadyTimestamp = 1000*timeToWait + getMillis();
  mBusy = 1;
  return status;
}

const float* SDIRemoteSensor::values() {
  return (const float*)mValues;
}

float SDIRemoteSensor::value(int index) {
  if (index > 9) { return nan(""); }
  return mValues[index];
}

#endif
