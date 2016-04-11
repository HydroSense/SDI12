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


### `void SDIRemoteSensor::changeAddress(char newAddress)`
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




# `SDI12Sensor`
Slave mode for SDI-12.  Respond to master queries, returning data that the master requests.

### `SDI12Sensor::SDI12Sensor(SDIStream& stream, char addr)`
Initialize a sensor object.

#### Arguments
  * **stream**: SDIStream object that will send the data
  * **addr**: Initial address of the sensor


### `int SDI12Sensor::listen()`
Starts the active listening phase.  Blocks until `stop()` is called by a handler,
will call appropriate event handlers if they are registered.

#### Arguments
None.

#### Return Value
Will never return, if it does, will specify an error code.


### `int SDI12Sensor::setIdentification(SDI12DeviceIdentification& identification)`
Sets the identification information that the device should return if requested
by the datalogger.

#### Arguments
  * **identification**: Object with identification information in it

#### Return Value
Zero if successful, error code otherwise.


### `int SDI12Sensor::registerStartMeasurementHandler(SDIResponse (*handler)(void))`
Registers a function to handle a request to start measurement.

#### Arguments
  * **handler**: Event handler function, needs to return an `SDIResponse`

#### Return Value
Zero if successful, error code otherwise.


### `int SDI12::registerStartAltMeasurementHandler(SDIResponse (*handler)(int altno))`
Registers a function to handle a request to start an alternative measurement.

#### Arguments
  * **handler**: Event handler function, needs to return an `SDIResponse`. Takes an integer that is the alternative number.

#### Return Value
Zero if successful, error code otherwise.


### `int SDI12::registerGetDataHandler(float* (*handler)(void))`
Registers a function to handle a request for data.

#### Arguments
  * **handler**: Event handler function, needs to return an `SDIResponse`

#### Return Value
Zero if successful, error code otherwise.
