#ifndef __SDI_BUS_CONTROLLER_HPP
#define __SDI_BUS_CONTROLLER_HPP

#define SDI_MAX_RESPONSE_TIME 15

//class SDIRemoteSensor{};

enum SDIBusError {
  OK,
  BUSY,
  ADDRESS_IN_USE,
  BAD_ADDRESS,
  NO_SPACE,
  TIMEOUT,
  UNREGISTERED_SENSOR,
  RESPONSE_ERROR
};
extern SDIBusError SDIBusErrno;

struct SDIDeviceIdentification{
  char addr[2];
  char sdiVersion[3];
  char vendor[9];
  char modelNum[7];
  char sensorVersion[4];
  char optional[14];
};

class SDIBusController {
//  friend class SDIRemoteSensor;
private:
//  SDIRemoteSensor** mSensors;
  unsigned int mMaxSensorCount;
  unsigned int mSensorCount;
  int mFlowControlPin;

//  SDIRemoteSensor* findSensorFromAddress(char addr);
  void sendPreamble();
  void setBufferWrite();
  void setBufferRead();

  // private constructor

public:
  SDIBusController(int flowControlPin, unsigned int maxSensors);

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
