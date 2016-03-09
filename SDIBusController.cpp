#ifndef __SDI_BUS_CONTROLLER_CPP
#define __SDI_BUS_CONTROLLER_CPP

// XXX: remove on product
#include <iostream>
using namespace std;

#include <stdlib.h>

#include "SDIBusController.hpp"
#include "SDIRemoteSensor.hpp"

SDIBusError SDIBusErrno;

/* Private Members */
SDIBusController::SDIBusController(SDIStream& sdiStream, int serialOutputPin, int flowControlPin):
  mSdiStream(sdiStream) {
  mSerialOutputPin = serialOutputPin;
  mFlowControlPin = flowControlPin;

  pinMode(mFlowControlPin, OUTPUT);

  // put tristate into high impedence mode
  mSdiStream->setBufferRead();
}

/* Public Members */
void SDIBusController::begin() {
  mSdiStream->begin();

  // ensure buffer is in high impedence mode
  mSdiStream->setBufferRead();
}

void SDIBusController::end(void) {
  mSdiStream->end();

  // put into high impedence mode
  mSdiStream->setBufferRead();
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

int SDIBusController::addressQuery(char *outAddr) {
  mSdiStream->setBufferWrite();
  mSdiStream->sendPreamble();
  mSdiStream->write('?');
  mSdiStream->write('!');
  mSdiStream->setBufferRead();

  // expected: addr, <CR>, <LF>
  char exp[2] = {'\r', '\n'};

  int numDelays = 0;
  while( mSdiStream->available() < 3){
      if( ++numDelays == SDI_SENSOR_RESPONSE_TIME_MS ){
          // TIME OUT
          SDIBusErrno = TIMEOUT;
          cout << "Failure - no device detected" << endl;
          return -1;
      }
      delay(1);
  }
  char newAddr = mSdiStream->read();
  for(int i=0; i<2; i++){
      if(mSdiStream->read() != exp[i]){
          cout << "Failure" << endl;
          SDIBusErrno = RESPONSE_ERROR;
          return -1;
      }
  }

  *outAddr = newAddr; // write new address

  SDIBusErrno = OK;
  return 0;
}

int SDIBusController::acknowledgeActive(char addr) {
  if (!this->isValidAddress(addr)) {
    SDIBusErrno = BAD_ADDRESS;
    return -1;
  }

  // write data out the serial
  mSdiStream->setBufferWrite();
  mSdiStream->sendPreamble();
  mSdiStream->write(addr);
  mSdiStream->write('!');
  mSdiStream->setBufferRead();

  // expected: sensor addr, <CR>, <LF>
  char exp[3] = {addr, '\r', '\n'};

  int numDelays = 0;
  while( mSdiStream->available() < 3){
      if( ++numDelays == SDI_SENSOR_RESPONSE_TIME_MS ){
          // TIME OUT - set error variable
          SDIBusErrno = TIMEOUT;
          cout << "Failure" << endl;
          return -1;
      }
      delay(1);
  }

  // sequentially compare each byte to expected
  for(int i=0; i<3; i++){
      if( mSdiStream->read() != exp[i] ){
          // incorrect response - set error variable
          SDIBusErrno = RESPONSE_ERROR;
          cout << "Failure" << endl;
          return -1;
      }
  }

  SDIBusErrno = OK;
  return 0;
}

int SDIBusController::identify(char addr, struct SDIDeviceIdentification* devInfo){

  mSdiStream->setBufferWrite();
  mSdiStream->sendPreamble();

  mSdiStream->write(addr);
  mSdiStream->write('I');
  mSdiStream->write('!');

  setBufferRead();

  int numDelays  = 0;
  while (mSdiStream->available() < 22) {
    if( ++numDelays == SDI_SENSOR_RESPONSE_TIME_MS ){
        // TIME OUT - set error variable
        SDIBusErrno = TIMEOUT;
        cout << "Failure" << endl;
        return -1;
    }
  }

  //Sensor address
  devInfo->addr[0] = mSdiStream->read();

  //SDI12 version number
  devInfo->sdiVersion[0] = mSdiStream->read();
  devInfo->sdiVersion[1] = mSdiStream->read();

  //Vendor identification
  for (int i = 0; i < 8; i++){
    devInfo->vendor[i] = mSdiStream->read();
  }

  //Sensor model number
  for (int j = 0; j < 6; j++){
    devInfo->modelNum[j] = mSdiStream->read();
  }

  //Sensor version
  for (int k = 0; k < 3; k++){
    devInfo->sensorVersion[k] = mSdiStream->read();
  }

  //Optional field
  int term = 0;
  int optInd = 0;
  while (!term && optInd < 13) {
    if (mSdiStream->available()) {
      devInfo->optional[optInd] = mSdiStream->read();
      if (devInfo->optional[optInd] == '\r') {//Add support for \n?
        devInfo->optional[optInd] = '\0';
        term = 1;
      }
      optInd++;
    }
  }
  return 0;
}

int SDIBusController::refresh(char addr, int altno, int* waitTime, int* numExpected) {
  if (!this->isValidAddress(addr)) {
    SDIBusErrno = BAD_ADDRESS;
    return -1;
  }

    mSdiStream->setBufferWrite();
    mSdiStream->sendPreamble();
    mSdiStream->write(addr);
    mSdiStream->write('C');
    if(altno > 0 && altno < 10){
       mSdiStream->write((char) (altno + '0'));
    }
    mSdiStream->write('!');
    mSdiStream->setBufferRead();

    // expected: atttnn<CR><LF>

    int numDelays = 0;
    while( mSdiStream->available() < 8){
       if( ++numDelays == SDI_SENSOR_RESPONSE_TIME_MS ){
           // TIME OUT
           SDIBusErrno = TIMEOUT;
           cout << "Failure - no device detected" << endl;
           return -1;
       }
       delay(1);
    }
    mSdiStream->read(); // address

    char time[3], meas[2];
    for(int i=0; i<3; i++){
       time[i] = mSdiStream->read();
    }
    for(int i=0; i<2; i++){
       meas[i] = mSdiStream->read();
    }

    // Last 2 characters: <CR><LF>
    char exp[2] = {'\r', '\n'};
    for(int i=0; i<2; i++){
        if(mSdiStream->read() != exp[i]){
            cout << "Failure" << endl;
            SDIBusErrno = RESPONSE_ERROR;
            return -1;
        }
    }

    *waitTime = 100*((int) (time[0] - '0')) + 10*((int) (time[1] - '0')) + ((int) (time[2] - '0'));
    *numExpected = 10*((int) (meas[0] - '0')) + ((int) (meas[1] - '0'));

    SDIBusErrno = OK;
    return 0;
}

int SDIBusController::getData(char addr, float* buffer, int numExpected) {
  /* known as Send Date in SDI-12 specification v1.3 */

  if (!this->isValidAddress(addr)) {
    SDIBusErrno = BAD_ADDRESS;
    return -1;
  }

  mSdiStream->setBufferWrite();
  this->sendPreamble();

  int numReceived = 0;
  char charBuffer[10] = {'\0'};
  int charBufferIndex;
  for (char iChr='0'; numReceived < numExpected && iChr <= '9'; iChr++) {
    mSdiStream->setBufferWrite();
    mSdiStream->write(addr);
    mSdiStream->write('D');
    mSdiStream->write(iChr);
    mSdiStream->write('!');
    mSdiStream->setBufferRead();

    // ensure the response address is correct
    while(mSdiStream->available() < 1);
    char responseAddress = mSdiStream->read();
    if (responseAddress != addr) {
      SDIBusErrno = RESPONSE_ERROR;
      return -1;
    }

    // read the sign
    charBufferIndex = 0;
    while(!mSdiStream->available());
    charBuffer[charBufferIndex++] = mSdiStream->read();

    // keep reading in digits until we get a sign
    char chr = '\0';
    while (chr != '\n') {
      while (chr != '+' && chr != '-' && chr != '\r') {
        while(!mSdiStream->available());
        chr = mSdiStream->read();
        charBuffer[charBufferIndex++] = chr;
      }
      charBufferIndex--;          // decrement the counter becuase we saved the chr when we returned

      // add the null terminator and then put through atoi
      charBuffer[charBufferIndex] = '\0';
      buffer[numReceived++] = atof(charBuffer);

      // go to the top of the loop
      if (chr == '\r') {
        // wait for the newline, if not newline then there is a bus error
        while(!mSdiStream->available());
        chr = mSdiStream->read();
        if (chr != '\n') {
          SDIBusErrno = RESPONSE_ERROR;
          return -1;
        }

        continue;
      }

      // put the sign back in the beginning of the buffer
      // blank out chr because it is used as a delimiter
      charBuffer[0] = chr;
      charBufferIndex = 1;
      chr = '\0';
    }
  }
}

int SDIBusController::changeAddress(char oldAddr, char newAddr) {
  if (!this->isValidAddress(oldAddr) || !this->isValidAddress(newAddr)) {
    SDIBusErrno = BAD_ADDRESS;
    return -1;
  }

  // write data out the serial
  mSdiStream->setBufferWrite();
  this->sendPreamble();
  mSdiStream->write(oldAddr);
  mSdiStream->write('A');
  mSdiStream->write(newAddr);
  mSdiStream->write('!');
  mSdiStream->setBufferRead();

  // expected: new sensor addr, <CR>, <LF>
  char exp[3] = {newAddr, '\r', '\n'};

  int numDelays = 0;
  while( mSdiStream->available() < 3){
      if( ++numDelays == SDI_SENSOR_RESPONSE_TIME_MS ){
          // TIME OUT - set error variable
          SDIBusErrno = TIMEOUT;
          cout << "Failure - timeout" << endl;
          return -1;
      }
      delay(1);
  }

  // sequentially compare each byte to expected
  for(int i=0; i<3; i++){
      if( mSdiStream->read() != exp[i] ){
          // incorrect response - set error variable
          SDIBusErrno = RESPONSE_ERROR;
          cout << "Failure - response error" << endl;
          return -1;
      }
  }

  SDIBusErrno = OK;
  return 0;
}

int SDIBusController::respondToAcknowledgeActive(char addr) {
  mSdiStream->write(addr);
  mSdiStream->write('\r');
  mSdiStream->write('\n');

  SDIBusErrno = OK;
  return 0;
}

//respond to send identification

int SDIBusController::respondToChangeAddress(char addr){
  mSdiStream->write(addr);
  mSdiStream->write('\r');
  mSdiStream->write('\n');

  SDIBusErrno = OK;
  return 0;
}

int SDIBusController::respondToAddressQuery(char addr){
  mSdiStream->write(addr);
  mSdiStream->write('\r');
  mSdiStream->write('\n');

  SDIBusErrno = OK;
  return 0;
}

int SDIBusController::respondToRefresh(char addr, int altno){
  mSdiStream->write(addr);

  mSdiStream->write('1');
  mSdiStream->write('2');
  mSdiStream->write('3');

  mSdiStream->write('4');
  mSdiStream->write('5');

  mSdiStream->write('\r');
  mSdiStream->write('\n');

  SDIBusErrno = OK;
  return 0;
}

#endif
