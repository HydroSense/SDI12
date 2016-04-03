#include "SDIStream.hpp"
#include "SDIRemoteSensor.hpp"

SDIRemoteSensor::SDIRemoteSensor(SDIStream& stream, char address):
  mStream(stream) {

}

// status functions
int SDIRemoteSensor::busy() {

}

// modification functions
void SDIRemoteSensor::changeAddress(char newAddress) {

}

// measurement triggers
int SDIRemoteSensor::startMeasurement() {

}
int SDIRemoteSensor::startAltMeasurement(int altNo) {

}

// getters
float* SDIRemoteSensor::getData() {

}
