
String meas = "0M!"; // measure command
String send = "0D0!"; // send data (back to host) command

String longtest = "aMC!";

int i = 0;

void setup(){
    Serial.begin(9600);
    Serial1.begin(1200);
    delay(1000);
}

void loop(){
    Serial1.print("hello");
    delay(1000);
}
