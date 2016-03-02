#include <stdio.h>
#include <iostream>
using namespace std;

#include <Arduino.h>
#include <SDIBusController.hpp>
#include <SDIRemoteSensor.hpp>

#define FLOW_CONTROL_PIN 13

int main(int argc, char** argv) {
  char addrPtr;

  cout << "Started test..." << endl;

  cout << "connecting to domain socket...";
  Serial1.connectDomainSocket();
  cout << "done!" << endl;

  SDIBus.begin(FLOW_CONTROL_PIN, 0);

  int res = SDIBus.acknowledgeActive('0');
  if (res < 0) {
    cout << "ERR: " << SDIBusErrno << endl;
  }

  //send identification test...

  // res = SDIBus.changeAddress('1');
  // if (res < 0) {
  //     cout << "ERR: " << SDIBusErrno << endl;
  // }

  res = SDIBus.addressQuery(&addrPtr);
  if (res < 0) {
      cout << "ERR: " << SDIBusErrno << endl;
  }
}
