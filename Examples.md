# Examples
This document contains some examples of the use case of this library.

## SDI12Master

### Example 1: Simple Data Measurement
This program initializes a master node and reads all data from address 0.

```C
#define FLOW_CONTROL_PIN 10

SDIBusController sdiBus(FLOW_CONTROL_PIN, 2);
SDIRemoteSensor genericSensor('A');
DecagonSensor decagon('X');

void setup() {
  SDIBus.register(genericSensor);
  SDIBus.register(decagon);
  SDIBus.begin(FLOW_CONTROL_PIN);



  Serial.begin(9600)
}

void loop() {
  SDIBusController.eventLoop();

  if (decagon.available()) {
    int temp = decagon.temperature();

    Serial.write("temp: ");
    Serial.writeln(temp);

    decagon.refresh();
  }
  if (genericSensor.available()) {
    float measurement = genericSensor.value(1);

    Serial.write("measurement: ");
    Serial.writeln(measurement);

    measurement.refresh('1');
  }
}
```

## SDI12Slave

### Example 1: Simple Wait for Response

```C
#define FLOW_CONTROL_PIN 10
SDI12Slave sdiSlave(FLOW_CONTROL_PIN);

void setup() {
  sdiSlave.begin();
}

void loop() {
  char cmd = sdiSlave.listen();     // wait for a command signal
  sdiSlave.ack(10, 2);

  ...                               // read the sensor data

  sdiSlave.beginTransmission();
  sdiSlave.write(sensorVal1);
  sdiSlave.write(sensorVal2);
  sdiSlave.endTransmission();
}
```

### Example 2: Multiple Data Request Commands

```C
char cmd = sdiSlave.listen();
if (cmd == '0') {
  sdiSlave.ack(1, 1);  

  ...                               // read sensors

  sdiSlave.beginTransmission();
  sdiSlave.write(sensorVal);
  sdiSlave.endTransmission();
} else if (cmd == '1') {
  sdiSlave.ack(0, 3);

  ...                               // read sensors

  sdiSlave.beginTransmission();
  sdiSlave.write(sensorVal1);
  sdiSlave.write(sensorVal2);
  sdiSlave.write(-5);
  sdiSlave.endTransmission();
} else {
  sdiSlave.ack(0, 0);
}

```
