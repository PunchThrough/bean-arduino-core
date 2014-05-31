/*
Turn green LED up to 11.

*/

void setup() 
{
   // initialize serial communication at 57600 bits per second:
   Serial.begin(57600);
}

// the loop routine runs over and over again forever:

void loop() 
{

   uint8_t green = 255;

   Bean.setLedGreen(green);

   // Our work here is done. Sleep a while.

   while (1)
   {
     Bean.sleep(200000);
   }

}
