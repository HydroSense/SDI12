#include <SoftwareSerial.h>

#define DATAPIN 9

SoftwareSerial mySerial(10, DATAPIN); // RX, TX

String meas = "0M!"; // measure command
String send = "0D0!"; // send data (back to host) command

String longtest = "aMC!";

int i = 0;

void setup(){
    Serial.begin(9600);
    mySerial.begin(1200);
    delay(1000);
}

void loop(){
    if(i++ == 100){
        i = 0;
        mySerial.print("hello");
    }
    for(unsigned int j = 0; j<10000; j++);
//    delay(1);
}
