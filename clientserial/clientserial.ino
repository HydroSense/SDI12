
int bled = 9;
int led = 0;

void setup(){
    Serial1.begin(1200, SERIAL_8N1);
    TXLED0; // SDI-12 RXEN 
    Serial.begin(9600);
    Serial.println("Starting.");

    pinMode(bled, OUTPUT);
    int led = 0;
    digitalWrite(bled, 0);
    delay(100);
    digitalWrite(bled, 1);
}
int i = 0;

void loop(){
    while(Serial1.available() > 0){
        int input = Serial1.read();
        if(input > 0)
            Serial.write(input);
    }
    Serial.println();
    delay(200);
}

