
void setup(){
    pinMode(4, OUTPUT);

    pinMode(1, OUTPUT);
    digitalWrite(4, 0); // output state of buffer
    Serial.begin(9600);
}


void loop(){
    Serial1.end();

    Serial.println("Sending message: 0M!");

    digitalWrite(1, 0);
    delayMicroseconds(12000);
    digitalWrite(1, 1);
    delayMicroseconds(8330);


    Serial1.begin(1200, SERIAL_7E1);

    Serial1.write("0M!\n");
    
    delay(500);
}
