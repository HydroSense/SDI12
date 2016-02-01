
int bled = 9;
int led = 0;

void setup(){
    Serial1.begin(1200);
    TXLED0; // SDI-12 RXEN 
    Serial.begin(9600);
    Serial.println("Starting.");

    pinMode(bled, OUTPUT);
    int led = 0;
    digitalWrite(bled, 0);
    delay(100);
    digitalWrite(bled, 1);
}

void loop(){
    if(Serial1.available()){
//        Serial.print(Serial1.available(), DEC);
//        Serial.println(" bytes available.");
        char input[256] = {}; // should zero each char.
        int i = 0; // counter
        while(Serial1.available() > 0){
            led = 1-led;
            digitalWrite(bled, led);
            input[i] = Serial1.read();
            delay(50);
            i++;
        }
        digitalWrite(bled, led);
        Serial.print("Raw from serial:");
        Serial.println(input);
        Serial.print("Non-inverted ASCII: ");
        Serial.println(input);
    }
}

