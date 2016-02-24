#ifndef __SDI_REMOTE_SENSOR_HPP
#define __SDI_REMOTE_SENSOR_HPP

#include "SDIBusController.hpp"

class SDIRemoteSensor {
private:
  char mAddr;
  unsigned long mReadyTimestamp;

  int mAvailable;
  int mBusy;
  float[10] mValues;

public:
  SDIRemoteSensor(char addr);

  // control functions
  int changeAddress(char newAddr);
  char getAddress(void);

  // measurement functions
  int available(void);
  int refresh(void);
  int refresh(int altno);

  // functions to get out data
  const float* values(void);
  const float value(int index);
};

#endif
