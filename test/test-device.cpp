#include <iostream>
#include <string.h>
using namespace std;

#include <unistd.h>

#include <Arduino.h>
#include <SDIBusController.hpp>

#define BUFFER_SIZE 256

int main(int argc, char** argv) {
  cout << "Started device..." << endl;
  cout << "Listening for client..." << endl;

  Serial1.serveDomainSocket();
  SDIBus.begin(0, 1);

  // wait for a command
  char buffer[BUFFER_SIZE] = {'\0'};
  char inChar = '\0';

  for (int i=0; inChar != '!'; i++) {
    // wait for a character
    while(!Serial1.available()) {
      delay(1);
    }

    // save away
    inChar = Serial1.read();
    buffer[i] = inChar;
  }

  cout << "BUFFER: " << buffer << endl;

  if (strcmp(buffer, "0!") == 0) {
    cout << "Responding..." << endl;
    SDIBus.respondToAcknowledgeActive('0');
  } else {
    cout << "IDK what that was, quitting" << endl;
  }

  cout << "Quitting in 10 seconds..." << endl;
  sleep(10);
}
