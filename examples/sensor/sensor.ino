/*
   Example sketch illustrating the sensor-side API
   Colin Heinzmann 4-11-2016
*/

#include "SDI.h"

// create device
SDISensor sensor(SDISerial, 'a');

// define handlers
SDIResponse startMeasurementHandler(void);
float* getDataHandler(void);

// data that measurements will go into
float values[5];

void setup(){
  sensor.registerStartMeasurementHandler(startMeasurementHandler);
  sensor.registerGetDataHandler(getDataHandler);
}

void loop(){
  sensor.listen();
}

SDIResponse startMeasurementHandler(void) {
  // data will take 1 second to collect, will be 5 points
  SDIResponse res;
  res.time = 1;
  res.points = 5;

  // start the measurement

  return res;
}

float* getDataHandler(void) {
  // get each of the measurement values
  for(int i=0; i < 5; i++) {
    values[i] = i;
  }

  return values;
}
