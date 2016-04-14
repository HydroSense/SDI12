#ifndef __SDI_REMOTE_SENSOR_CPP
#define __SDI_REMOTE_SENSOR_CPP

#include <stdlib.h>

#include "SDIStream.hpp"
#include "SDIRemoteSensor.hpp"

SDIRemoteSensor::SDIRemoteSensor(SDIStream &bus, char addr):
  mySDIStream(bus) {

  this->address = addr;

  timeReady = 0;
}

//TODO implement this
int SDIRemoteSensor::listen(){
  return 0; // testing
}

// TODO ensure it's ok not to deep copy
int SDIRemoteSensor::setIdentification(SDIDeviceIdentification &id){
  memcpy(&(this->mySDIDeviceIdentification), &id, sizeof(struct SDIDeviceIdentification));
  return 0;
}

// TODO figure this out
int SDIRemoteSensor::registerStartMeasurementHandler(int (*handler)(void)){
  int res = handler();
  if(res == SDIResponse::OK){
    return 0;
  }
  return -1; // error
}

int SDIRemoteSensor::registerStartAltMeasurementHandler(int altno, int (*handler)(int altno)){
  handler(altno);
  return 0;
}

int SDIRemoteSensor::registerGetDataHandler(float *(*handler)(void)){
  handler();
  return 0;
}

/*
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

*/

struct SDIDeviceIdentification SDIRemoteSensor::getMySDIDeviceIdentification(){
  return this->mySDIDeviceIdentification;
}

#endif
