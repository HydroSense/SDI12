#ifndef __SDI_BUS_CONTROLLER_HPP
#define __SDI_BUS_CONTROLLER_HPP

#include <Arduino.h>

#include "SDIStream.hpp"

// protocol timing constants
#define SDI_BREAK_TIME_MS           12
#define SDI_MARKING_TIME_MS         9
#define SDI_SENSOR_RESPONSE_TIME_MS 15
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
  char addr[2] = {};
  char sdiVersion[3] = {};
  char vendor[9] = {};
  char modelNum[7] = {};
  char sensorVersion[4] = {};
  char optional[14] = {};
};

class SDIBusController {
//  friend class SDIRemoteSensor;
private:
  SDIStream& mSdiStream;
  int mSerialOutputPin;
  int mFlowControlPin;

  bool isValidAddress(char addr);

public:
  SDIBusController(SDIStream& serial, int serialOutputPin, int flowControlPin);

  void begin(void);
  void end(void);



//  int register(SDIRemoteSensor& sensor);
//  void eventLoop(void);

  int addressQuery(char *outAddr); // Use when there is only 1 sensor connected
  int acknowledgeActive(char addr);
  int identify(char addr, struct SDIDeviceIdentification* devInfo);

  int refresh(char addr, int altno, int *waitTime, int *numExpected);
  int getData(char addr, float* buffer, int numExpected);
  int changeAddress(char oldAddr, char newAddr);

  int respondToAcknowledgeActive(char addr);

  int respondToSendIdentificationNoOpt(char addr);
  int respondToSendIdentificationPartialOpt(char addr);
  int respondToSendIdentificationFullOpt(char addr);

  int respondToChangeAddress(char addr);
  int respondToAddressQuery(char addr);
  int respondToRefresh(char addr, int altno);
};

#endif
