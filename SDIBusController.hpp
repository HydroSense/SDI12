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

struct SDIDeviceInfo {
  char vendor[9];
  char model[7];
  char version[4];
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
  public:SDIBusController();

public:
  void begin(int flowControlPin, unsigned int maxSensors);
  void end(void);

//  int register(SDIRemoteSensor& sensor);
//  void eventLoop(void);

  int addressQuery(char *outAddr); // Use when there is only 1 sensor connected
  int acknowledgeActive(char addr);
  int identify(char addr, SDIDeviceInfo* devInfo);

  int refresh(char addr, int altno);
  int getData(char addr, float* buffer);
  int changeAddress(char oldAddr, char newAddr);

  int respondToAcknowledgeActive(char addr);
  int respondToChangeAddress(char addr);
  int respondToAddressQuery(char addr);
  int respondToRefresh(char addr, int altno);
};

// singleton declaration
extern SDIBusController SDIBus;

#endif
