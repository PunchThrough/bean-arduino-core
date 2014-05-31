
// Notify the client over serial when a digital pin state changes

static int d0 = 0;
static int d1 = 1;
static int d2 = 2;
static int d3 = 3;
static int d4 = 4;
static int d5 = 5;
static uint8_t pinMap[] = {d0,d1,d2,d3,d4,d5};
static uint8_t pinValues[] = {0, 0, 0, 0, 0, 0};

// the setup routine runs once when you press reset:
void setup() 
{
  // initialize serial communication at 57600 bits per second:
  Serial.begin(57600);
  
  // Digital pins
  pinMode(d0, INPUT_PULLUP);  
  pinMode(d1, INPUT_PULLUP); 
  pinMode(d2, INPUT_PULLUP);  
  pinMode(d3, INPUT_PULLUP); 
  pinMode(d4, INPUT_PULLUP);
  pinMode(d5, INPUT_PULLUP);
 
}

void loop()
{
 bool notify = false;
 
 for ( int i = 0; i < 6; i++ )
 {
   uint8_t pinState = digitalRead( pinMap[i]);
   
   if ( pinState != pinValues[ i ] )
   {
     notify = true;
     pinValues[ i ] = pinState;
   } 
 }
 
 if ( notify )
 {
   uint8_t buffer[1];
   buffer[0] = 0;
   
   for ( int i = 0; i < 6; i++ )
   {
     // Make a bitfield.  1 pin per bit.
     buffer[0] |= pinValues [ i ] << i; 
   }
   
   Serial.write( buffer, 1 );
 }
  
 // Sleep for half a second before checking the pins again  
 Bean.sleep(500);  
}
