#ifndef __SDI_BUS_CONTROLLER_CPP
#define __SDI_BUS_CONTROLLER_CPP

#include <stdlib.h>

#include "SDIStream.hpp"
#include "SDIBusController.hpp"

SDIBusError SDIBusErrno;

/* Private Members */
SDIBusController::SDIBusController(SDIStream &serial):
  mySDIStream(serial) {
    /*
  mSerialOutputPin = serialOutputPin;
  mFlowControlPin = flowControlPin;

  pinMode(mFlowControlPin, OUTPUT);
  */

  // put tristate into high impedence mode
  mySDIStream.setBufferRead();
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
  mySDIStream.setBufferWrite();
  mySDIStream.sendPreamble();
  mySDIStream.write('?');
  mySDIStream.write('!');
  mySDIStream.flush();
  mySDIStream.setBufferRead();

  // expected: addr, <CR>, <LF>
  char exp[2] = {'\r', '\n'};

  int numDelays = 0;
  while( mySDIStream.available() < 3){
      if( ++numDelays == SDI_SENSOR_RESPONSE_TIME_MS ){
          // TIME OUT
          SDIBusErrno = TIMEOUT;
          /* cout << "Failure - no device detected" << endl; */
          return -2;
      }
      delay(1);
  }
  char newAddr = mySDIStream.read();
  for(int i=0; i<2; i++){
      if(mySDIStream.read() != exp[i]){
          /* cout << "Failure" << endl; */
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
    /* cout<<"Bad address"<<endl; */
    SDIBusErrno = BAD_ADDRESS;
    return -1;
  }

  // write data out the serial
  mySDIStream.setBufferWrite();
  mySDIStream.sendPreamble();
  mySDIStream.write(addr);
  mySDIStream.write('!');
  mySDIStream.flush();
  mySDIStream.setBufferRead();

  // expected: sensor addr, <CR>, <LF>
  char exp[3] = {addr, '\r', '\n'};

  int numDelays = 0;
  while( mySDIStream.available() < 3){
      if( ++numDelays == SDI_SENSOR_RESPONSE_TIME_MS ){
          // TIME OUT - set error variable
          SDIBusErrno = TIMEOUT;
          /* cout << "Failure" << endl; */
          return -1;
      }
      delay(1);
  }

  // sequentially compare each byte to expected
  for(int i=0; i<3; i++){
      if( mySDIStream.read() != exp[i] ){
          // incorrect response - set error variable
          SDIBusErrno = RESPONSE_ERROR;
          /* cout << "Failure" << endl; */
          return -1;
      }
  }

  SDIBusErrno = OK;
  return 0;
}

int SDIBusController::identify(char addr, struct SDIDeviceIdentification* devInfo){
  if (!this->isValidAddress(addr)) {
    SDIBusErrno = BAD_ADDRESS;
    return -1;
  }

  mySDIStream.setBufferWrite();
  mySDIStream.sendPreamble();

  mySDIStream.write(addr);
  mySDIStream.write('I');
  mySDIStream.write('!');
  mySDIStream.flush();

  mySDIStream.setBufferRead();

  int numDelays  = 0;
  while (mySDIStream.available() < 22) {
    if( ++numDelays == SDI_SENSOR_RESPONSE_TIME_MS ){
        // TIME OUT - set error variable
        SDIBusErrno = TIMEOUT;
        /* cout << "Failure" << endl; */
        return -1;
    }

    delay(1);
  }

  //Sensor address
  devInfo->addr[0] = mySDIStream.read();

  //SDI12 version number
  devInfo->sdiVersion[0] = mySDIStream.read();
  devInfo->sdiVersion[1] = mySDIStream.read();

  //Vendor identification
  for (int i = 0; i < 8; i++){
    devInfo->vendor[i] = mySDIStream.read();
  }

  //Sensor model number
  for (int j = 0; j < 6; j++){
    devInfo->modelNum[j] = mySDIStream.read();
  }

  //Sensor version
  for (int k = 0; k < 3; k++){
    devInfo->sensorVersion[k] = mySDIStream.read();
  }

  //Optional field

  int term = 0;
  int optInd = 0;
  while (!term && optInd < 14) {
    if (mySDIStream.available()) {
      devInfo->optional[optInd] = mySDIStream.read();
      if (devInfo->optional[optInd] == '\r') {//Add support for \n?
        devInfo->optional[optInd] = '\0';
        term = 1;
      }
      optInd++;
    }
  }
  if(mySDIStream.available()){
    mySDIStream.read(); // throw away the rest
  }
  return 0;
}

int SDIBusController::refresh(char addr, int altno, int* waitTime, int* numExpected) {
  // Set altno to -1 for 'regular' refresh
  if (!this->isValidAddress(addr)) {
    SDIBusErrno = BAD_ADDRESS;
    return -1;
  }

    mySDIStream.setBufferWrite();
    mySDIStream.sendPreamble();
    mySDIStream.write(addr);
    mySDIStream.write('C');
    if(altno > 0 && altno < 10){
       mySDIStream.write((char) (altno + '0'));
    }
    mySDIStream.write('!');
    mySDIStream.flush();
    mySDIStream.setBufferRead();

    // expected: atttnn<CR><LF>

    int numDelays = 0;
    while( mySDIStream.available() < 8){
       if( ++numDelays == SDI_SENSOR_RESPONSE_TIME_MS ){
           // TIME OUT
           //Serial.println("TIMEOUT in refresh");
           SDIBusErrno = TIMEOUT;
           /* cout << "Failure - no device detected" << endl; */
           return -1;
       }
       delay(1);
    }
    mySDIStream.read(); // address

    char time[3], meas[2];
    for(int i=0; i<3; i++){
       time[i] = mySDIStream.read();
    }
    for(int i=0; i<2; i++){
       meas[i] = mySDIStream.read();
    }

    // Last 2 characters: <CR><LF>
    char exp[2] = {'\r', '\n'};
    for(int i=0; i<2; i++){
        if(mySDIStream.read() != exp[i]){
            /* cout << "Failure" << endl; */
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


  int numReceived = 0;
  char charBuffer[10] = {'\0'};
  int charBufferIndex;
  for (char iChr='0'; numReceived < numExpected && iChr <= '9'; iChr++) {
    mySDIStream.setBufferWrite();
    mySDIStream.sendPreamble();
    mySDIStream.write(addr);
    mySDIStream.write('D');
    mySDIStream.write(iChr);
    mySDIStream.write('!');
    mySDIStream.flush();
    mySDIStream.setBufferRead();

    // ensure the response address is correct
    while(mySDIStream.available() < 1);
    char responseAddress = mySDIStream.read();
    if (responseAddress != addr) {
      SDIBusErrno = RESPONSE_ERROR;
      return -1;
    }

    // read the sign
    charBufferIndex = 0;
    while(!mySDIStream.available());
    charBuffer[charBufferIndex++] = mySDIStream.read();

    // keep reading in digits until we get a sign
    char chr = '\0';
    while (chr != '\n') {
      while (chr != '+' && chr != '-' && chr != '\r') {
        while(!mySDIStream.available());
        chr = mySDIStream.read();
        charBuffer[charBufferIndex++] = chr;
      }
      charBufferIndex--;          // decrement the counter becuase we saved the chr when we returned

      // add the null terminator and then put through atoi
      charBuffer[charBufferIndex] = '\0';
      buffer[numReceived++] = atof(charBuffer);

      // go to the top of the loop
      if (chr == '\r') {
        // wait for the newline, if not newline then there is a bus error
        while(!mySDIStream.available());
        chr = mySDIStream.read();
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
  return 0;
}

int SDIBusController::changeAddress(char oldAddr, char newAddr) {
  if (!this->isValidAddress(oldAddr) || !this->isValidAddress(newAddr)) {
    SDIBusErrno = BAD_ADDRESS;
    return -1;
  }

  // write data out the serial
  mySDIStream.setBufferWrite();
  mySDIStream.sendPreamble();
  mySDIStream.write(oldAddr);
  mySDIStream.write('A');
  mySDIStream.write(newAddr);
  mySDIStream.write('!');
  mySDIStream.flush();
  mySDIStream.setBufferRead();

  // expected: new sensor addr, <CR>, <LF>
  char exp[3] = {newAddr, '\r', '\n'};

  int numDelays = 0;
  while( mySDIStream.available() < 3){
      if( ++numDelays == SDI_SENSOR_RESPONSE_TIME_MS ){
          // TIME OUT - set error variable
          SDIBusErrno = TIMEOUT;
          /* cout << "Failure - timeout" << endl; */
          return -1;
      }
      delay(1);
  }

  // sequentially compare each byte to expected
  for(int i=0; i<3; i++){
      if( mySDIStream.read() != exp[i] ){
          // incorrect response - set error variable
          SDIBusErrno = RESPONSE_ERROR;
          /* cout << "Failure - response error" << endl; */
          return -1;
      }
  }

  SDIBusErrno = OK;
  return 0;
}

#endif
