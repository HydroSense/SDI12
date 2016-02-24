#ifndef __SDI_BUS_CONTROLLER_HPP
#define __SDI_BUS_CONTROLLER_HPP

#define SDI_MAX_RESPONSE_TIME 15

class SDIRemoteSensor;

enum SDIBusError {
  OK,
  BUSY,
  NOT_FOUND,
  ADDRESS_IN_USE,
  BAD_ADDRESS,
  NO_SPACE,
  TIMEOUT,
  UNREGISTERED_SENSOR
};
SDIBusError SDIBusErrno;

struct SDIDeviceInfo {
  char vendor[9];
  char model[7];
  char version[4];
  char optional[14];
};

class SDIBusController {
  friend class SDIRemoteSensor;
private:
  SDIRemoteSensor** mSensors;
  unsigned int mMaxSensorCount;
  unsigned int mSensorCount;
  int mFlowControlPin;

  SDIRemoteSensor* findSensorFromAddress(char addr);
  void sendPreamble();
  void setBufferWrite();
  void setBufferRead();

  // private constructor
  SDIBusController();

public:
  void begin(int flowControlPin, unsigned int maxSensors);
  void end(void);

  int register(SDIRemoteSensor& sensor);
  void eventLoop(void);

  char addressQuery(void);
  int acknowledgeActive(char addr);
  int identify(char addr, SDIDeviceInfo* devInfo);

  int refresh(char addr, int altno);
  int getData(char addr, float* buffer);
  int changeAddress(char oldAddr, char newAddr);
};

// singleton declaration
extern SDIBusController SDIBus;

#endif
