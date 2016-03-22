/*
 * This is an example sketch for use with the observer role functionality in the Bean.
 * The Bean will listen for undirected advertisements and print them out to the serial terminal.
 * When used in conjunction with a sketch like customAdvert.ino, the Bean will change it's LED to match the LED
 * Of the custom advert Bean.
 * 
 * Note that the heavy use of the print and println functionality slows the search down.  Removing all instances
 * Of print and println will speed up the process
 */

void setup() {
  Serial.begin();
  
}

uint8_t expectedPacket[] = {
  // general discoverable mode advertises indefinitely
  0x02,   // length of this data
  GAP_ADTYPE_FLAGS,
  GAP_ADTYPE_FLAGS_GENERAL | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,

  // two-byte broadcast
  0x03,   // length of this data including the data type byte
  GAP_ADTYPE_MANUFACTURER_SPECIFIC,
  0xAC   // arbitrary data so we can make sure this is the Bean we're looking for
};

void loop() {  
  uint32_t start = millis();

    ObseverAdvertisementInfo info;
    int res = Bean.getObserverMessage(&info, 2000);
    if(res == 1) {      
      Serial.print(info.eventType);
      Serial.print(',');
      Serial.print(info.addrType);
      Serial.print(',');
      Serial.print("addr: ");
      for(int i = 0; i < 6; i ++) {
        Serial.print(info.addr[i]);
        Serial.print(',');
      }
      Serial.print("  advData: ");
      for(int i = 0; i < info.dataLen; i ++) {
        Serial.print(info.advData[i], HEX);
        Serial.print(',');
      }
      Serial.println();
      
      //check to see if this packet is form the custom advertiser
      bool isEqual = true;
      for(int i = 0; i < sizeof(expectedPacket) - 1; i++) { 
        if(expectedPacket[i] != info.advData[i])
        {
          isEqual = false;
        }
      }
      if(isEqual) {
         Serial.println("Custom adv found");
         if(info.advData[6] == 0) {
           Bean.setLedGreen(60);
         }
         else {
           Bean.setLedGreen(0);
         }
      }
    }
}