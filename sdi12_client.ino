// Colby Rome 11-29-2015
/*
   SDI-12 Client sketch

   Responds to commands based on the SDI-12 v1.3 Specification.
   The default sensor address is 'b'.

   Wake the sensor by pulling the data line high for at least 12
   milliseconds. If the first character transmitted is the sensor address,
   the sensor will respond to the command.

   The baud rate for SDI-12 is 1200. All characters transmitted on the
   SDI-12 bus must be printable ASCII characters.

   Page 8 of the SDI-12 v1.3 specification lists all commands.

*/

void setup()
{
    // Begin serial at 1200 baud

    // Set up interrupt
}

void loop()
{
}
