
int bled = 9;
int led = 0;

void setup(){
    Serial1.begin(1200, SERIAL_7E1); // SDI-12 runs at 1200 baud
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
        char input[256] = {}; // should zero each char
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
        Serial.println(input[0], HEX);
        Serial.println(input[1], HEX);
        Serial.println(input[2], HEX);
        Serial.println(input[3], HEX);
        Serial.println(input[4], HEX);
        Serial.println(input[5], HEX);
        Serial.println(input[6], HEX);
        Serial.println(input[7], HEX);
        Serial.print("Non-inverted ASCII: ");
        Serial.println(input);
        Serial.print("Inverted ASCII: ");
        input[0] = ~input[0] & 0xFE;
        input[1] = ~input[1] & 0xFE;
        input[2] = ~input[2] & 0xFE;
        input[3] = ~input[3] & 0xFE;
        Serial.println(input[0]);
        Serial.println(input[1]);
        Serial.println(input[2]);
        Serial.println(input[3]);
    }
}

