# SDI-12 Arduino Library

# `SDIRemoteSensor`
Management class for hardware SDI-12 master mode.  Queries slave devices to get
data out of them.

### `SDIRemoteSensor::SDIRemoteSensor(SDIStream& stream, char address)`
Initializes and SDI-12 slave sensor device.

#### Arguments
  * **stream**: SDIStream that handles the communication
  * **address**: SDI address

### `int SDIRemoteSensor::busy()`
Checks to see if the device is in the middle of a measurement.

#### Arguments
None.

#### Return Value
1 if the device is busy (has sent off a startMeasurement command).

0 if the device is not busy.


### `void SDIRemoteSensor::changeAddress(char old, char new)`
Changes a device address from `old` to `new`.

#### Arguments
  * **old**: old address
  * **new**: new address

#### Return Value
None.



### `int SDIRemoteSensor::startMeasurement()`
Start an asynchronous measurement.

#### Arguments
None.

#### Return Value
If <0, error
If >=0, the number of seconds until the measurement is ready

### `int SDIRemoteSensor::startAltMeasurement(int altNo)`
Starts an asynchronous alternative measurement.

#### Arguments
  * **altNo**: alternative index number (1-9)

### `float* SDIRemoteSensor::getData()`
Gets array of measurements.

#### Arguments
None.

#### Return Value
Returns `NULL` if the sensor isn't ready.



### `static char SDIRemoteSensor::querySensorAddress()`
Queries the bus to see if there is a sensor on it.

#### Arguments
None.

#### Return Value
Returns the character of the address or **'\0'** if no sensor was found




# `SDI12Slave`
Slave mode for SDI-12.  Respond to master queries, returning data that the master requests.

### `SDI12Slave::SDI12Slave(int pin)`
Initialize a slave object.

#### Arguments
  * **pin**: digital I/O pin used for flow control


### `void SDI12Slave::ack(unsigned int seconds, unsigned int numResponses)`
Acknowledge an SDI-12 data request.  Lets the master know how long it will take
for the slave to respond and what data is going to respond.

#### Arguments
  * **seconds**: number of seconds it will take to process the request
  * **numResponses**: number of measurement values the slave will respond with

#### Return Value
None.


### `int SDI12Slave::begin()`
Starts the SDI-12 object, binds the hardware serial port and digial I/O pins.

#### Arguments
None.

#### Return Value
None.


### `void SDI12Slave::beginTransmission()`
Starts an SDI-12 transmission, performs synchronization to ensure data is being
written at the same time.

#### Arguments
None.

#### Return Value
None.


### `void SDI12Slave::end()`
Stops the SDI-12 slave device and unbinds hardware serial device and I/O pin.

#### Arguments
None.

#### Return Value
None.


### `void SDI12Slave::endTransmission()`
Ends an SDI-12 transmission.

#### Arguments
None.

#### Return Value
None.


### `char SDI12Slave::listen()`
Wait for a data request command from the master.  Function blocks until a command
has been received.  Also allows other internal SDI-12 protocol commands to be
processed by the library.

#### Arguments
None.

#### Return Value
The command number that has been received.  Set to '0' if this is a standard
measurement request ('aM!').


### `int SDI12Slave::write(int value)`
Appends the data to the response string.  Does not send the value

#### Arguments
  * **value**: Value to send

#### Return Value
0 if append is successful

<0 if out of buffer space


### `int SDI12Slave::write(float value)`
Appends the data to the response string.

#### Arguments
  * **value**: Value to send
