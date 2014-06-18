
// Notify the client over serial when a temperature reading changes

static int8_t temp = 0;

// the setup routine runs once when you press reset:
void setup() 
{
  // initialize serial communication at 57600 bits per second:
  Serial.begin(57600); 
}

void loop()
{
 bool notify = false;
 
 int8_t newTemp = Bean.getTemperature();
 
 if ( newTemp != temp )
 {
   temp = newTemp;
   
   Serial.print("Temperature: ");
   Serial.println(temp);
 }
  
 // Sleep for a second before reading the temperature again  
 Bean.sleep(1000);  
}
