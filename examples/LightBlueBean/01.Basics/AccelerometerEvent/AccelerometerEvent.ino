void setup() {
  // put your setup code here, to run once:

  Bean.enableAccelEvent( SINGLE_TAP_EVENT | HIGH_G_EVENT );
}

void loop() {
  // put your main code here, to run repeatedly:

  if ( Bean.checkAccelEvent( SINGLE_TAP_EVENT ) )
  {
    // Turn on the LED 
    Bean.setLed( 0xff, 0x00, 0x00 );
  }
  
  if ( Bean.checkAccelEvent( HIGH_G_EVENT ) )
  {
    // Turn off the LED
    Bean.setLed( 0x00, 0x00, 0x00 );
  }
 
  Bean.sleep(10000);
}
