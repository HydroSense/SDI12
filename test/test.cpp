#include <stdio.h>
#include <iostream>
using namespace std;

#include <Arduino.h>
#include <SDIBusController.hpp>
#include <SDIRemoteSensor.hpp>

#define FLOW_CONTROL_PIN 13

int main(int argc, char** argv) {
  SDIBus.begin(FLOW_CONTROL_PIN, 0);

  SDIBus.acknowledgeActive('0');
}
