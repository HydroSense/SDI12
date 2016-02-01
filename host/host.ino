#include <SDI12.h>

#define DATAPIN 9
SDI12 mySDI12(DATAPIN);

String meas = "0M!"; // measure command
String send = "0D0!"; // send data (back to host) command

String longtest = "aMC!";

void setup(){
    Serial.begin(9600);
    mySDI12.begin();
    delay(1000);
}

void loop(){
    mySDI12.sendCommand("A");
    delay(1000);
}
