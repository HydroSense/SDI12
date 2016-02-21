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
