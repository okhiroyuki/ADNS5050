//Add the ADNS5050 Library to the sketch.
#include <adns5050.h>

//Name the ADNS5050, and tell the sketch which pins are used for communication
ADNS5050 mouse(2,3,4);

//This value will be used to store information from the mouse registers.
unsigned char value=0;

void setup()
{
    //Create a serial output.
    Serial.begin(57600);

    //Initialize the ADNS5050
    mouse.begin();
    delay(100);
    //A sync is performed to make sure the ADNS5050 is communicating
    mouse.sync();
}

void loop()
{
    //The DELTA_X_REG and DELTA_Y_REG store the x and y movements detected by the sensor

    //Read the DELTA_X_REG register and Serial.print
    Serial.print(mouse.read(DELTA_X_REG));
    Serial.print(",");

    //Read the DELTA_Y_REG register and Serial.print
    Serial.print(mouse.read(DELTA_Y_REG));
    Serial.println();

    delay(10);    
}
