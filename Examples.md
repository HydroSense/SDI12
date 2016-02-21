# Examples
This document contains some examples of the use case of this library.

## SDI12Master

### Example 1: Simple Data Measurement
This program initializes a master node and reads all data from address 0.

```C
#define FLOW_CONTROL_PIN 10
SDI12Master sdiMaster(FLOW_CONTROL_PIN);

char data[256];

void setup() {
  sdiMaster.begin();

  Serial.begin(9600);
}

void loop() {
  // start the measurement
  sdiMaster.startMeasurement('0');

  // wait while we aren't available
  while (sdiMaster.available() == 0);

  // save the data in the buffer
  sdiMaster.read(data, 256);

  // print out results
  Serial.println("Received SDI12 Data");
  Serial.println(data);
}
```

### Example 2: Additional Measurement

```C
sdiMaster.startAdditionalMeasurement('0', '1');

while (sdiMaster.available() == 0);

sdiMaster.read(data, 256);

Serial.println("Received SDI12 Data");
Serial.println(data);
```

### Example 3: Wake Then Query

```C
sdiMaster.wake();
sdiMaster.startMeasurement('0');

while(sdiMaster.available() != 0);

sdiMaster.read(data);
```

### Example 4: Wake, Change Address, Query
```C
sdiMaster.wake();
sdiMaster.changeAddress('0', '5');
sdiMaster.startMeasurement('5');
...
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
