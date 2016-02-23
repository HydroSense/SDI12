#ifndef __SDI_BUS_CONTROLLER_HPP
#define __SDI_BUS_CONTROLLER_HPP

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

class SDIBusController {
  friend class SDIRemoteSensor;
private:
  SDIRemoteSensor** mSensors;
  unsigned int mMaxSensorCount;
  unsigned int mSensorCount;
  int mFlowControlPin;

  SDIRemoteSensor* findSensorFromAddress(char addr);
public:
  SDIBusController(int flowControlPin, unsigned int maxSensors);

  void begin(void);
  void end(void);

  int register(SDIRemoteSensor& sensor);
  void eventLoop(void);

  int refresh(char addr, int altno);
  int getData(char addr, float* buffer);
  int changeAddress(char oldAddr, char newAddr);
};

#endif
