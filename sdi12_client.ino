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

   The commands implemented in this sketch are:
   -- Name --                -- Command --
   - Acknowledge Active         b!
   - Send Identification        bI!
   - Change Address             bAx! // change address to x
   - Address Query              ?! // responds with current address
   - Start Measurement          bM!
   - Send Data                  bD0! // 0 through 9

*/

// Global variables
volatile unsigned long endtime;
char addr = 'b'; // default address is character 'b'
char command[3] = {0, 0, 0};

void setup()
{
    // Begin serial at 1200 baud
    Serial.begin(1200);

    // Set up interrupt
    attachInterrupt();
    interrupts(); // enable interrupts

}

void loop()
{
    if(isValidBegin()){
        while(Serial.available() == 0); // wait until receive a character
        char targaddr = Serial.read();
        // If command is addressed to addr or to all sensors (char '?')
        if((targaddr == addr) || (targaddr == '?')){
            // The max number of additional characters we receive is 3.
            while(Serial.peek() != '!'); // Read until '!' char received
            int i = 0;
            char recv;
            while((recv = Serial.read()) != -1){
                command[i] = recv;
            }

            // Now we have the full command
            switch(command[i]){
                case '!':
                    acknowledgeActive();
                    break;
                case 'I':
                    sendID();
                    break;
                case 'A':
                    changeAddress();
                    break;
                case 'M':
                    startMeasurement();
                    break;
                case 'D':
                    sendData();
                    break;
            }
        }
    }

}

bool isValidBegin()
{
    /* Continuously monitors the data line for 12ms. If at any point 
       the data line goes low, the function returns false. Otherwise,
       returns true.
    */

    endtime = millis() +12;
    while(millis() < endtime){ // execute for 12 milliseconds
        // If dataline is low
        return false;
    }
    while(dataline is high); // busy wait until line goes low
    return true;
}

