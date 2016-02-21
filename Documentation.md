# SDI-12 Arduino Library

# `SDI12Master`
Management class for hardware SDI-12 master mode.  Queries slave devices to get
data out of them.

## `SDI12Master::SDI12Master(int pin)`
Initializes an SDI-12 master device.
### Arguments
  * **pin**: Digital I/O pin used for flow control.

## `int SDI12Master::active()`
Checks to see if this instance of SDI12Master has started.
### Arguments
None.
### Return Value
1 if the device is active.

0 if the device is inactive.

## `void SDI12Master::begin()`
Starts the object, activates serial hardware, and binds the flow control pin.
### Arguments
None.
### Return Value
None.

## `int SDI12Master::busy()`
Checks to see if the device is in the middle of a measurement.
### Arguments
None.
### Return Value
1 if the device is busy (has sent off a startMeasurement command).

0 if the device is not busy.

## `void SDI12Master::changeAddress(char old, char new)`
Changes a device address from `old` to `new`
### Arguments
  * **old**: old address
  * **new**: new address
### Return Value
None.

## `void SDI12Master::end()`
Stops the object, electrically disconnects master from the control line.
### Arguments
None.
### Return Value
None.

## `int SDI12Master::read(char* buffer, size_t size)`
Reads one measurement value from the SDI-12 device into the buffer.
### Arguments
  * **data**: pointer to a buffer to store the data
  * **size**: size, in bytes, of the provided buffer
### Return Value
If <0, then the measurement isn't ready yet.

If >=0, then the number of bytes read.

## `int SDI12Master::readAll(char* buffer, size_t size)`
Reads all measurement values from the SDI-12 device into the buffer.
### Arguments
  * **data**: pointer to a buffer to store the data
  * **size**: size, in bytes, of the provided buffer
### Return Value
If <0, then the measurement isn't ready yet.

If >=0, then the number of bytes read.

## `int SDI12Master::startMeasurement(char address)`
Starts serial measurement with device.
### Arguments
  * **address**: target address of device you wish to measure
### Return Value
Integer, number of seconds it is expected to take until the measurement is ready.

## `void SDI12Master::wake()`
Sends a break command which wakes all devices on the line.  Function call should
block for 12ms before returning.
### Arguments
None.
### Return Value
None.




# `SDI12Slave`
Slave mode for SDI-12.  Respond to master queries, returning data that the master requests.

## `SDI12Slave::SDI12Slave(pin)`
Initializes an instance of SDI12Slave.

### Arguments
  * **pin**: pin used for flow control management
