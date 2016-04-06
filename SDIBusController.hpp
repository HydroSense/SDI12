#ifndef __SDI_BUS_CONTROLLER_HPP
#define __SDI_BUS_CONTROLLER_HPP

#include <Arduino.h>

#include "SDISerial.hpp"

// protocol timing constants
#define SDI_BREAK_TIME_MS           12
#define SDI_MARKING_TIME_MS         9
#define SDI_SENSOR_RESPONSE_TIME_MS 15 //???? this is time for initial response... tricky
#define SDI_SENSOR_MAXIMUM_TIME_MS  780

//class SDIRemoteSensor{};

enum SDIBusError {
  OK=0,
  BUSY=1,
  ADDRESS_IN_USE=2,
  BAD_ADDRESS=3,
  NO_SPACE=4,
  TIMEOUT=5,
  UNREGISTERED_SENSOR=6,
  RESPONSE_ERROR=7
};
extern SDIBusError SDIBusErrno;

struct SDIDeviceIdentification{
  char addr[2] = {0};
  char sdiVersion[3] = {0};
  char vendor[9] = {0};
  char modelNum[7] = {0};

  char sensorVersion[4] = {0};
  char optional[14] = {0};
};

class SDIBusController {
//  friend class SDIRemoteSensor;
private:
  SDIStream &mySDIStream;

  bool isValidAddress(char addr);

public:
  SDIBusController(SDIStream &serial);

  virtual int addressQuery(char *outAddr); // Use when there is only 1 sensor connected
  virtual int acknowledgeActive(char addr);
  virtual int identify(char addr, struct SDIDeviceIdentification* devInfo);

  virtual int refresh(char addr, int altno, int *waitTime, int *numExpected);
  virtual int getData(char addr, float* buffer, int numExpected);
  virtual int changeAddress(char oldAddr, char newAddr);

  virtual int respondToAcknowledgeActive(char addr);

  //virtual int respondToSendIdentificationNoOpt(char addr);
  //virtual int respondToSendIdentificationPartialOpt(char addr);
  //virtual int respondToSendIdentificationFullOpt(char addr);

  virtual int respondToChangeAddress(char addr);
  virtual int respondToAddressQuery(char addr);
  virtual int respondToRefresh(char addr, int altno);
};

#endif
