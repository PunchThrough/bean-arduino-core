Description

Sets the red LED to an intensity between off (0) and fully ON (255).

Syntax

void setLedRed(uint8 red);

Returns

Nothing. Absolutely nothing.

Example

/*
Turn red LED up to 11.

*/

void setup() 
{
   // initialize serial communication at 57600 bits per second:
   Serial.begin(57600);
}

// the loop routine runs over and over again forever:

void loop() 
{

   uint8_t red = 255;

   Bean.setLedRed(red);

   // Our work here is done. Sleep a while.

   while (1)
   {
     Bean.sleep(200000);
   }

}
 
