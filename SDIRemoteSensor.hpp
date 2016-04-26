#ifndef SDI_REMOTE_SENSOR_H
#define SDI_REMOTE_SENSOR_H

#define SDI_MAX_DATA 8

#include <Arduino.h>
#include "SDISerial.hpp"

enum SDIError {
  OK=0,
  BUSY=1,
  ADDRESS_IN_USE=2,
  BAD_ADDRESS=3,
  NO_SPACE=4,
  TIMEOUT=5,
  UNREGISTERED_SENSOR=6,
  RESPONSE_ERROR=7
};

struct SDIDeviceIdentification{
  char addr[2] = {0};
  char sdiVersion[3] = {0};
  char vendor[9] = {0};
  char modelNum[7] = {0};

  char sensorVersion[4] = {0};
  char optional[14] = {0};
};

typedef char* SDIResponse;

class SDIRemoteSensor {
private:
  SDIStream &mySDIStream;
  char address;
  struct SDIDeviceIdentification mySDIDeviceIdentification;
  bool isMyAddress(char otherAddress);
  SDIResponse (*startMeasurementHandler)(void);
  SDIResponse (*startAltMeasurementHandler)(int altno);
  SDIResponse (*getDataHandler)(void);
public:
  SDIRemoteSensor(SDIStream &bus, char addr);
  int listen();
  int setIdentification(SDIDeviceIdentification &id);
  int registerStartMeasurementHandler(SDIResponse (*handler)(void));
  int registerStartAltMeasurementHandler(SDIResponse (*handler)(int altno));
  int registerGetDataHandler(SDIResponse (*handler)(void));

  // For unit tests
  struct SDIDeviceIdentification getMySDIDeviceIdentification();

};


#endif
