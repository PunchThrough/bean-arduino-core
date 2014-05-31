/*
Get the current LED RGB intensity levels.
*/
 
void setup()
{
   // initialize serial communication at 57600 bits per second:
   Serial.begin(57600);
}
 
// the loop routine runs over and over again forever:
void loop()
{
 
   uint8_t ledRed = 0;
 
   ledRed = Bean.getLedRed();
 
   // if it's not on, turn it on
   if ( 0 == ledRed )
   {
     Bean.setLedRed(255);
   }
 
   Bean.sleep(10000);
}
