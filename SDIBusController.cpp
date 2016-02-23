#ifndef __SDI_BUS_CONTROLLER_CPP
#define __SDI_BUS_CONTROLLER_CPP

#include <stdlib.h>
#include <Arduino.h>

#include "SDIBusController.hpp"
#include "SDIRemoteSensor.hpp"

/* Public Members */
SDIBusController::SDIBusController(int flowControlPin, unsigned int maxSensors) {
  mMaxSensorCount = maxSensors;
  mSensorCount = 0;

  mFlowControlPin = flowControlPin;
  mSensors = calloc(mMaxSensors, sizeof(SDIRemoteSensor*));
}

void SDIBusController::begin(void) {
}

void SDIBusController::end(void) {
}

int SDIBusController::register(SDIRemoteSensor& sensor) {
  SDIBusErrno = OK;

  if (mSensorCount >= mMaxSensorCount) {
    SDIBusErrno = NO_SPACE;
    return -1;
  }

  if(this->findSensorFromAddress(sensor.getAddress()) != NULL) {
    SDIBusErrno = ADDRESS_IN_USE;
    return -1;
  }

  mSensors[mSensorCount++] = &sensor;
  return 0;
}

void SDIBusController::eventLoop() {
  unsigned long currentTime = getMillis();

  for (int i = 0; i < mSensorCount; i++) {
    SDIRemoteSensor* sensorPtr = mSensor[i];

    // if the sensor is in the middle of a request and we are ready, then process
    if (sensorPtr->busy() && currentTime >= sensorPtr->mReadyTimestamp) {
      sensorPtr->mBusy = 0;

      this->getData(sensorPtr);
    }
  }
}

int SDIBusController::refresh(char addr, int altno) {
  return -1;
}

int SDIBusController::getData(char addr, float* buffer) {
  return -1;
}

int SDIBusController::changeAddress(char oldAddr, char newAddr) {
  return -1;
}

#endif
