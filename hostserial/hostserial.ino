#include <SoftwareSerial.h>

#define DATAPIN 9

SoftwareSerial mySerial(10, DATAPIN); // RX, TX

String meas = "0M!"; // measure command
String send = "0D0!"; // send data (back to host) command

String longtest = "aMC!";

void setup(){
    Serial.begin(9600);
    mySerial.begin(1200);
    delay(1000);
}

void loop(){
    mySerial.print("hello");
    delay(1000);
}
