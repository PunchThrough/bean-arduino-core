/*
 * This is an example sketch for use with the observer role functionality in the Bean.
 * The Bean will listen for undirected advertisements and print them out to the serial terminal.
 * When used in conjunction with a sketch like customAdvert.ino, the Bean will change it's LED to match the LED
 * Of the custom advert Bean.
 */

void setup() {
  Serial.begin();
  
}

void loop() {  
  uint32_t start = millis();

    ObseverAdvertisementInfo info;
    int res = Bean.getObserverMessage(&info, 2000);
    if(res == 1)
    {      
      Serial.print(info.eventType);
      Serial.print(',');
      Serial.print(info.addrType);
      Serial.print(',');
      Serial.print("addr: ");
      for(int i = 0; i < 6; i ++)
      {
        Serial.print(info.addr[i]);
        Serial.print(',');
      }
      Serial.print("  advData: ");
      for(int i = 0; i < info.dataLen; i ++)
      {
        Serial.print(info.advData[i], HEX);
        Serial.print(',');
      }
      Serial.println();
      
      
      if(info.advData[0] == 0x02 &&
         info.advData[1] == GAP_ADTYPE_FLAGS &&
         info.advData[2] == (GAP_ADTYPE_FLAGS_GENERAL | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED) &&
         info.advData[3] == 0x03 && 
         info.advData[4] == GAP_ADTYPE_MANUFACTURER_SPECIFIC) &&
         info.advData[5] == 0xAC
      {
         if(info.advData[6] == 0)
         {
           Bean.setLedGreen(60);
         }
         else
         {
           Bean.setLedGreen(0);
         }
      }
    }
}