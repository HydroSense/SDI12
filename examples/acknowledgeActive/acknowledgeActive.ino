/*
   Example sketch for testing a sensor is active.
*/

#include "SDIBusController.hpp"

#define FLOW_CONTROL_PIN 4

void setup(){
    SDIBus.begin(FLOW_CONTROL_PIN, 2);
    Serial.begin(9600);
}

void loop(){
    int res = SDIBus.acknowledgeActive('0');
    if(res != 0){
        Serial.println("Error: didn't respond");
    }
    else{
        Serial.println("Success.");
    }

    delay(1000);
}
