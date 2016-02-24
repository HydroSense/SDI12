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

    pinMode(mFlowControlPin, OUTPUT);
    Serial1.begin(1200, SERIAL_7E1);
}

void SDIBusController::end(void) {

    Serial1.end();
    digitalWrite(mFlowControlPin, 1); // buffer is receiving
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

void sendPreamble() {
    // Set tri-state buffer to write mode
    digitalWrite(mFlowControlPin, 0);

    // Stop Serial1
    Serial1.end();

    digitalWrite(1, 0);
    delayMicroseconds(12000); // wait for 12 ms
    digitalWrite(1, 1);
    delayMicroseconds(8300); // wait for 8.3 ms
    
    // re-enable Serial1
    Serial1.begin(1200, SERIAL_7E1);
}

void SDIBusController::eventLoop() {
  unsigned long currentTime = getMillis();

  for (int i = 0; i < mSensorCount; i++) {
    SDIRemoteSensor* sensorPtr = mSensor[i];

    // if the sensor is in the middle of a request and we are ready, then process
    if (sensorPtr->mBusy && currentTime >= sensorPtr->mReadyTimestamp) {
      sensorPtr->mBusy = 0;
      int status = this->getData(sensorPtr->mAddr, sensorPtr->data);
      if (status < 0) {
        continue;
      }

      sensorPtr->mAvailable = 1;
    }
  }
}

char SDIBusController::addressQuery(void) {
  return 255;
}

int SDIBusController::acknowledgeActive(char addr) {
    sendPreamble();
    Serial1.write(addr+"!");

   Serial1.end();


  return -1;
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
