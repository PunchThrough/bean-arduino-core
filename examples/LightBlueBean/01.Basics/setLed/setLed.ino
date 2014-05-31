/*
Set LED to white (all colors on, intensity at 255).

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
   uint8_t green = 255;
   uint8_t blue = 255;

   Bean.setLed(red, green, blue);
 
   // Our work here is done.  Sleep a while.

   while (1)
   {
     Bean.sleep(200000);
   }
}
