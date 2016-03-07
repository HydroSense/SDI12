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
SDIBusController::SDIBusController(Stream& serial, int serialOutputPin, int flowControlPin) {
  mSerial = &serial;
  mSerialOutputPin = serialOutputPin;
  mFlowControlPin = flowControlPin;

  pinMode(mFlowControlPin, OUTPUT);

  // put tristate into high impedence mode
  this->setBufferRead();
}

/* Public Members */
void SDIBusController::begin() {
  mSerial->begin(1200, SERIAL_7E1);

  // ensure buffer is in high impedence mode
  this->setBufferRead();
}

void SDIBusController::end(void) {
  mSerial->end();

  // put into high impedence mode
  this->setBufferRead();
}

void SDIBusController::sendPreamble() {
  /*
  Sends the required preamble according to the SDI-12 specification
  */

    // Stop mSerial
    mSerial->end();

    pinMode(mSerialOutputPin, OUTPUT);
    digitalWrite(mSerialOutputPin, 1);
    delay(SDI_BREAK_TIME_MS);
    digitalWrite(mSerialOutputPin, 0);
    delay(SDI_MARKING_TIME_MS);

    // re-enable mSerial
    mSerial->begin(1200, SERIAL_7E1);
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

int SDIBusController::addressQuery(char *outAddr) {
  this->setBufferWrite();
  this->sendPreamble();
  mSerial->write('?');
  mSerial->write('!');
  this->setBufferRead();

  // expected: addr, <CR>, <LF>
  char exp[2] = {'\r', '\n'};

  int numDelays = 0;
  while( mSerial->available() < 3){
      if( ++numDelays == SDI_SENSOR_RESPONSE_TIME_MS ){
          // TIME OUT
          SDIBusErrno = TIMEOUT;
          cout << "Failure - no device detected" << endl;
          return -1;
      }
      delay(1);
  }
  char newAddr = mSerial->read();
  for(int i=0; i<2; i++){
      if(mSerial->read() != exp[i]){
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
  this->setBufferWrite();
  sendPreamble();
  mSerial->write(addr);
  mSerial->write('!');
  this->setBufferRead();

  // expected: sensor addr, <CR>, <LF>
  char exp[3] = {addr, '\r', '\n'};

  int numDelays = 0;
  while( mSerial->available() < 3){
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
      if( mSerial->read() != exp[i] ){
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

  this->sendPreamble();

  mSerial->write(addr);
  mSerial->write('I');
  mSerial->write('!');

  setBufferRead();

  int numDelays  = 0;
  while (mSerial->available() < 22) {
    if( ++numDelays == SDI_SENSOR_RESPONSE_TIME_MS ){
        // TIME OUT - set error variable
        SDIBusErrno = TIMEOUT;
        cout << "Failure" << endl;
        return -1;
    }
  }

  //Sensor address
  devInfo->addr[0] = mSerial->read();

  //SDI12 version number
  devInfo->sdiVersion[0] = mSerial->read();
  devInfo->sdiVersion[1] = mSerial->read();

  //Vendor identification
  for (int i = 0; i < 8; i++){
    devInfo->vendor[i] = mSerial->read();
  }

  //Sensor model number
  for (int j = 0; j < 6; j++){
    devInfo->modelNum[j] = mSerial->read();
  }

  //Sensor version
  for (int k = 0; k < 3; k++){
    devInfo->sensorVersion[k] = mSerial->read();
  }

  //Optional field
  int term = 0;
  int optInd = 0;
  while (!term && optInd < 13) {
    if (mSerial->available()) {
      devInfo->optional[optInd] = mSerial->read();
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

    this->setBufferWrite();
    this->sendPreamble();
    mSerial->write(addr);
    mSerial->write('C');
    if(altno > 0 && altno < 10){
       mSerial->write((char) (altno + '0'));
    }
    mSerial->write('!');
    this->setBufferRead();

    // expected: atttnn<CR><LF>

    int numDelays = 0;
    while( mSerial->available() < 8){
       if( ++numDelays == SDI_SENSOR_RESPONSE_TIME_MS ){
           // TIME OUT
           SDIBusErrno = TIMEOUT;
           cout << "Failure - no device detected" << endl;
           return -1;
       }
       delay(1);
    }
    mSerial->read(); // address

    char time[3], meas[2];
    for(int i=0; i<3; i++){
       time[i] = mSerial->read();
    }
    for(int i=0; i<2; i++){
       meas[i] = mSerial->read();
    }

    // Last 2 characters: <CR><LF>
    char exp[2] = {'\r', '\n'};
    for(int i=0; i<2; i++){
        if(mSerial->read() != exp[i]){
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

  this->setBufferWrite();
  this->sendPreamble();

  int numReceived = 0;
  char charBuffer[10] = {'\0'};
  int charBufferIndex;
  for (char iChr='0'; numReceived < numExpected && iChr <= '9'; iChr++) {
    this->setBufferWrite();
    mSerial->write(addr);
    mSerial->write('D');
    mSerial->write(iChr);
    mSerial->write('!');
    this->setBufferRead();

    // ensure the response address is correct
    while(mSerial->available() < 1);
    char responseAddress = mSerial->read();
    if (responseAddress != addr) {
      SDIBusErrno = RESPONSE_ERROR;
      return -1;
    }

    // read the sign
    charBufferIndex = 0;
    while(!mSerial->available());
    charBuffer[charBufferIndex++] = mSerial->read();

    // keep reading in digits until we get a sign
    char chr = '\0';
    while (chr != '\n') {
      while (chr != '+' && chr != '-' && chr != '\r') {
        while(!mSerial->available());
        chr = mSerial->read();
        charBuffer[charBufferIndex++] = chr;
      }
      charBufferIndex--;          // decrement the counter becuase we saved the chr when we returned

      // add the null terminator and then put through atoi
      charBuffer[charBufferIndex] = '\0';
      buffer[numReceived++] = atof(charBuffer);

      // go to the top of the loop
      if (chr == '\r') {
        // wait for the newline, if not newline then there is a bus error
        while(!mSerial->available());
        chr = mSerial->read();
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
  this->setBufferWrite();
  this->sendPreamble();
  mSerial->write(oldAddr);
  mSerial->write('A');
  mSerial->write(newAddr);
  mSerial->write('!');
  this->setBufferRead();

  // expected: new sensor addr, <CR>, <LF>
  char exp[3] = {newAddr, '\r', '\n'};

  int numDelays = 0;
  while( mSerial->available() < 3){
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
      if( mSerial->read() != exp[i] ){
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
  mSerial->write(addr);
  mSerial->write('\r');
  mSerial->write('\n');

  SDIBusErrno = OK;
  return 0;
}

//respond to send identification

int SDIBusController::respondToChangeAddress(char addr){
  mSerial->write(addr);
  mSerial->write('\r');
  mSerial->write('\n');

  SDIBusErrno = OK;
  return 0;
}

int SDIBusController::respondToAddressQuery(char addr){
  mSerial->write(addr);
  mSerial->write('\r');
  mSerial->write('\n');

  SDIBusErrno = OK;
  return 0;
}

int SDIBusController::respondToRefresh(char addr, int altno){
  mSerial->write(addr);

  mSerial->write('1');
  mSerial->write('2');
  mSerial->write('3');

  mSerial->write('4');
  mSerial->write('5');

  mSerial->write('\r');
  mSerial->write('\n');

  SDIBusErrno = OK;
  return 0;
}

#endif
