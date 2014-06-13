void setup()
{
 Serial.begin(57600);
}


void loop()
{
  uint16_t batteryV = 0;
  
  Bean.sleep(1000);
  
  batteryV = Bean.getBatteryVoltage();
  
  Serial.print("Battery voltage: ");
  Serial.println(batteryV, DEC);
  
  Bean.sleep(1000);
}
