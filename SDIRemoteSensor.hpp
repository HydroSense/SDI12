#ifndef SDI_REMOTE_SENSOR_H
#define SDI_REMOTE_SENSOR_H

#include "SDIStream.hpp"

class SDIRemoteSensor {
private:
  SDIStream& mStream;
  char mAddress;
public:
  SDIRemoteSensor(SDIStream& stream, char address);

  // status functions
  int busy();
  static char querySensorAddress();

  // modification functions
  void changeAddress(char newAddress);

  // measurement triggers
  int startMeasurement();
  int startAltMeasurement(int altNo);

  // getters
  float* getData();
}

#endif
