#include <Arduino.h>

#include "SDIStream.hpp"
#include "SDIRemoteSensor.hpp"

SDIRemoteSensor::SDIRemoteSensor(SDIBusController& bus, char address):
  mBus(bus) {

  mAddress = address;

  mBusy = false;
  mTimeReady = 0;
  mNumExpected = 0;
}

// status functions
int SDIRemoteSensor::busy() {
  if (!mBusy) {
    return 0;
  }

  // check to see if we are ready to get the data
  if (millis() > mTimeReady) {
    mBusy = false;
    mBus.getData(mAddress, mData, mNumExpected)
    return 0;
  } else {
    return 1;
  }
}

// modification functions
int SDIRemoteSensor::changeAddress(char newAddress) {
  return mBus.changeAddress(mAddress, newAddress);
}

// measurement triggers
unsigned long SDIRemoteSensor::startMeasurement() {
  if (this->busy()) {
    return -1;
  }

  mBus.refresh(mAddress, 0, &waitTime, &mNumExpected);

  mTimeReady = millis() + ((unsigned long)waitTime)*1000L;
  return mTimeReady;
}
unsigned long SDIRemoteSensor::startAltMeasurement(int altNo) {
  if (this->busy()) {
    return -1;
  }

  mBus.refresh(mAddress, altNo, &waitTime, &mNumExpected);

  mTimeReady = millis() + ((unsigned long)waitTime)*1000L;
  return mTimeReady;
}

// getters
float* SDIRemoteSensor::getData() {
  if (this->busy()) {
    return NULL;
  } else {
    return mData;
  }
}
