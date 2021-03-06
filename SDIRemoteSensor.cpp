#ifndef __SDI_REMOTE_SENSOR_CPP
#define __SDI_REMOTE_SENSOR_CPP

#include <stdlib.h>

#include "SDIStream.hpp"
#include "SDIRemoteSensor.hpp"

SDIRemoteSensor::SDIRemoteSensor(SDIStream &bus, char addr):
  mySDIStream(bus) {
  this->address = addr;
}

bool SDIRemoteSensor::isMyAddress(char otherAddress){
    return (this->address == otherAddress) || ('?' == otherAddress);
}

//TODO implement this
int SDIRemoteSensor::listen(){
    while(0 == mySDIStream.available()){
        delay(1);
    }
    delay(10);
    int numDelays = 0;
    // Now that we've received at least 1 character, wait until received "\r\n"
    while(mySDIStream.peek() != '!'){
        delay(10);
        numDelays++;
        if(numDelays > 10){ // timeout
            return -1;
        }
    }
    // Now we have the command. Read it into a buffer, then act on the command.
    int bufSize = mySDIStream.available()+1;
    char *cmd = new char[mySDIStream.available()+1];
    for(int i=0; i<bufSize-1; i++){
        cmd[i] = (char) mySDIStream.read();
    }
    cmd[bufSize] = '\0'; // null terminator
    printf("Received command: %s\n", cmd);
    if(isMyAddress(cmd[0])){
        printf("Addresses match\n");
        // The command was addressed to us.
        if(0 == strcmp(&cmd[1], "!")){
            // Acknowledge Active && Address Query
            //TODO write response
        } else if(0 == strcmp(&cmd[1], "I!")){
            // Send Identification
            // TODO write response
        } else if(0 == strcmp(&cmd[1], "C!")){
            // Start Measurement
            printf("Start Measurement Command Received\n");
            SDIResponse res = this->startMeasurementHandler();
            printf("From the handler, received: %s\n", res);
            // delete[] theResponse; // I don't know why this is an invalid pointer?
            // TODO: investigate numbers following C
        } else if(0 == strcmp(&cmd[1], "D0!")){
            // Send Data
            printf("Get Data Command Received\n");
            SDIResponse res = this->getDataHandler();
            printf("From the handler, received: %s\n", res);
            // TODO Investigate this (number following M could be 1-9)
        } else if(0 == strcmp(&cmd[1], "Ab!")){
            // Change Address
            // TODO Investigate how to extend past just changing address to 'b'.
        }
    } else{
        printf("Wrong Address.\n");
        return -1;
    }
  return 0; // testing
}

int SDIRemoteSensor::setIdentification(SDIDeviceIdentification &id){
  memcpy(&(this->mySDIDeviceIdentification), &id, sizeof(struct SDIDeviceIdentification));
  return 0;
}

// TODO figure this out
int SDIRemoteSensor::registerStartMeasurementHandler(SDIResponse (*handler)(void)){
    // Bind the startMeasurementHandler to the given function.
    this->startMeasurementHandler = handler;
  return 0;
}

int SDIRemoteSensor::registerStartAltMeasurementHandler(SDIResponse (*handler)(int altno)){
    this->startAltMeasurementHandler = handler;
  return 0;
}

int SDIRemoteSensor::registerGetDataHandler(SDIResponse (*handler)(void)){
  handler();
  this->getDataHandler = handler;
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
