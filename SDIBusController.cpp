#ifndef __SDI_BUS_CONTROLLER_CPP
#define __SDI_BUS_CONTROLLER_CPP

// XXX: remove on product
#include <iostream>
using namespace std;

#include <stdlib.h>
#include "Arduino.h"

#include "SDIBusController.hpp"
#include "SDIRemoteSensor.hpp"

SDIBusError SDIBusErrno;

/* Private Members */
SDIBusController::SDIBusController(int flowControlPin, unsigned int maxSensors) {
  mFlowControlPin = flowControlPin;
  mMaxSensorCount = maxSensors;
  mSensorCount = 0;

  // allocate memory for the sensors
  // mSensors = calloc(mMaxSensors, sizeof(SDIRemoteSensor*));
}

/* Public Members */
void SDIBusController::begin() {
    pinMode(mFlowControlPin, OUTPUT);
    Serial1.begin(1200, SERIAL_7E1);
}

void SDIBusController::end(void) {
    Serial1.end();
    digitalWrite(mFlowControlPin, 1); // buffer is receiving
}

/*
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
*/

void SDIBusController::sendPreamble() {
    // Set tri-state buffer to write mode
    setBufferWrite();

    // Stop Serial1
    Serial1.end();

    digitalWrite(mFlowControlPin, 0);
    delayMicroseconds(12000); // wait for 12 ms
    digitalWrite(mFlowControlPin, 1);
    delayMicroseconds(8330); // wait for 8.33 ms

    // re-enable Serial1
    Serial1.begin(1200, SERIAL_7E1);
}

void SDIBusController::setBufferWrite(){
    digitalWrite(mFlowControlPin, 0);
}

void SDIBusController::setBufferRead(){
    digitalWrite(mFlowControlPin, 1);
}

bool SDIBusController::isValidAddress(char addr) {
  if (addr <= '9' && addr >= '0') {
    return true;
  } else if (addr <= 'z' && addr >= 'a') {
    return true;
  } else if (addr <= 'Z' && addr >= 'A') {
    return true;
  }

  return false;
}

/*
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
*/

int SDIBusController::addressQuery(char *outAddr) {
  if (!this->isValidAddress(addr)) {
    SDIBusErrno = BAD_ADDRESS;
    return -1;
  }

  sendPreamble();

  Serial1.write('?');
  Serial1.write('!');
  setBufferRead();

  // expected: addr, <CR>, <LF>
  char exp[2] = {'\r', '\n'};

  int numDelays = 0;
  while( Serial1.available() < 3){
      if( ++numDelays == SDI_MAX_RESPONSE_TIME ){
          // TIME OUT
          SDIBusErrno = TIMEOUT;
          cout << "Failure - no device detected" << endl;
          return -1;
      }
      delay(1);
  }
  char newAddr = Serial1.read();
  for(int i=0; i<2; i++){
      if(Serial1.read() != exp[i]){
          cout << "Failure" << endl;
          SDIBusErrno = RESPONSE_ERROR;
          return -1;
      }
  }

  *outAddr = newAddr; // write new address
  cout << "Success" << endl;

  SDIBusErrno = OK;
  return 0;
}

int SDIBusController::acknowledgeActive(char addr) {
  if (!this->isValidAddress(addr)) {
    SDIBusErrno = BAD_ADDRESS;
    return -1;
  }

  sendPreamble();

  // write data out the serial
  Serial1.write(addr);
  Serial1.write('!');

  setBufferRead();

  // expected: sensor addr, <CR>, <LF>
  char exp[3] = {addr, '\r', '\n'};

  int numDelays = 0;
  while( Serial1.available() < 3){
      if( ++numDelays == SDI_MAX_RESPONSE_TIME ){
          // TIME OUT - set error variable
          SDIBusErrno = TIMEOUT;
          cout << "Failure" << endl;
          return -1;
      }
      delay(1);
  }

  // sequentially compare each byte to expected
  for(int i=0; i<3; i++){
      if( Serial1.read() != exp[i] ){
          // incorrect response - set error variable
          SDIBusErrno = RESPONSE_ERROR;
          cout << "Failure" << endl;
          return -1;
      }
  }

  cout << "Success" << endl;

  SDIBusErrno = OK;
 return 0;
}

int SDIBusController::refresh(char addr, int altno, int* waitTime, int* numExpected) {
  if (!this->isValidAddress(addr)) {
    SDIBusErrno = BAD_ADDRESS;
    return -1;
  }

    sendPreamble();

    Serial1.write(addr);
    Serial1.write('C');
    if(altno > 0 && altno < 10){
       Serial1.write((char) (altno + '0'));
    }
    Serial1.write('!');
    setBufferRead();

    // expected: atttnn<CR><LF>

    int numDelays = 0;
    while( Serial1.available() < 8){
       if( ++numDelays == SDI_MAX_RESPONSE_TIME ){
           // TIME OUT
           SDIBusErrno = TIMEOUT;
           cout << "Failure - no device detected" << endl;
           return -1;
       }
       delay(1);
    }
    Serial1.read(); // address

    char time[3], meas[2];
    for(int i=0; i<3; i++){
       time[i] = Serial1.read();
    }
    for(int i=0; i<2; i++){
       meas[i] = Serial1.read();
    }

    // Last 2 characters: <CR><LF>
    char exp[2] = {'\r', '\n'};
    for(int i=0; i<2; i++){
        if(Serial1.read() != exp[i]){
            cout << "Failure" << endl;
            SDIBusErrno = RESPONSE_ERROR;
            return -1;
        }
    }

    *waitTime = 100*((int) (time[0] - '0')) + 10*((int) (time[1] - '0')) + ((int) (time[2] - '0'));
    *numExpected = 10*((int) (meas[0] - '0')) + ((int) (meas[1] - '0'));

    cout << "Success" << endl;

    SDIBusErrno = OK;
    return 0;
}

int SDIBusController::getData(char addr, float* buffer, int numExpected) {
    return -1;
  if (!this->isValidAddress(addr)) {
    SDIBusErrno = BAD_ADDRESS;
    return -1;
  }

  int numReceived = 0;
  char charBuffer[10] = {'\0'};
  int charBufferIndex;
  for (char iChr='0'; numReceived <= numExpected && iChr <= '9'; iChr++) {
    Serial1.write(addr);
    Serial1.write('D');
    Serial1.write(iChr);
    Serial1.write('!');

    // ensure the response address is correct
    while(Serial1.available() < 1);
    char responseAddress = Serial1.read();
    if (responseAddress != addr) {
      SDIBusErrno = RESPONSE_ERROR;
      return -1;
    }

    // read the sign
    charBufferIndex = 0;
    while(!Serial1.available());
    charBuffer[charBufferIndex++] = Serial1.read();

    // keep reading in digits until we get a sign
    char chr = '\0';
    while (chr != '\r') {
      while (chr != '+' && chr != '-' && chr != '\r') {
        while(!Serial1.available());
        chr = Serial1.read();
        charBuffer[charBufferIndex++] = chr;
      }
      charBufferIndex--;          // decrement the counter becuase we saved the chr when we returned

      // go to the top of the loop
      if (chr == '\r') {
        // wait for the newline, if not newline then there is a bus error
        while(!Serial1.available());
        if (Serial1.read() != '\n') {
          SDIBusErrno = RESPONSE_ERROR;
          return -1;
        }

        continue;
      }

      // add the null terminator and then put through atoi
      charBuffer[charBufferIndex] = '\0';
      buffer[numReceived++] = atof(charBuffer);

      // put the sign back in the beginning of the buffer
      charBuffer[0] = chr;
      charBufferIndex = 1;
    }
  }
}

int SDIBusController::changeAddress(char oldAddr, char newAddr) {
  if (!this->isValidAddress(addr) || !this->isValidAddress(newAddr)) {
    SDIBusErrno = BAD_ADDRESS;
    return -1;
  }

  sendPreamble();

  // write data out the serial
  Serial1.write(oldAddr);
  Serial1.write('A');
  Serial1.write(newAddr);
  Serial1.write('!');

  setBufferRead();

  // expected: new sensor addr, <CR>, <LF>
  char exp[3] = {newAddr, '\r', '\n'};

  int numDelays = 0;
  while( Serial1.available() < 3){
      if( ++numDelays == SDI_MAX_RESPONSE_TIME ){
          // TIME OUT - set error variable
          SDIBusErrno = TIMEOUT;
          cout << "Failure - timeout" << endl;
          return -1;
      }
      delay(1);
  }

  // sequentially compare each byte to expected
  for(int i=0; i<3; i++){
      if( Serial1.read() != exp[i] ){
          // incorrect response - set error variable
          SDIBusErrno = RESPONSE_ERROR;
          cout << "Failure - response error" << endl;
          return -1;
      }
  }

  cout << "Success" << endl;

  SDIBusErrno = OK;
  return 0;
}

int SDIBusController::respondToAcknowledgeActive(char addr) {
  Serial1.write(addr);
  Serial1.write('\r');
  Serial1.write('\n');

  SDIBusErrno = OK;
  return 0;
}

//respond to send identification

int SDIBusController::respondToChangeAddress(char addr){
  Serial1.write(addr);
  Serial1.write('\r');
  Serial1.write('\n');

  SDIBusErrno = OK;
  return 0;
}

int SDIBusController::respondToAddressQuery(char addr){
  Serial1.write(addr);
  Serial1.write('\r');
  Serial1.write('\n');

  SDIBusErrno = OK;
  return 0;
}

int SDIBusController::respondToRefresh(char addr, int altno){
  Serial1.write(addr);

  Serial1.write('1');
  Serial1.write('2');
  Serial1.write('3');

  Serial1.write('4');
  Serial1.write('5');

  Serial1.write('\r');
  Serial1.write('\n');

  SDIBusErrno = OK;
  return 0;
}

#endif
