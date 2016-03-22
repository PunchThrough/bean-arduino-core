/*
 * This allows the Bean to act as an ANCS device.
 * ANCS is the Apple Notification Center Service. This allows a connected
 * Bluetooth device to receive information about iOS nofications. Bean can view
 * details about these notifications, e.g. email and SMS previews, or an
 * incoming phone call's number.
 *
 * For details on what different elements mean, please see the official
 * Apple ANCS documentation:
 * https://developer.apple.com/library/ios/documentation/CoreBluetooth/Reference/AppleNotificationCenterServiceSpecification/Appendix/Appendix.html#//apple_ref/doc/uid/TP40013460-CH3-SW2
 *
 * Upload this sketch to Bean, power cycle your Bean, then connect to an iOS
 * device. Pair with Bean using pairing code 000000. Once you're connected, use
 * Bean Console for iOS to view data from your iOS device live as it arrives.
 *
 * When you disconnect from an Bean in ANCS mode, you'll need to manually forget
 * it from the iOS Bluetooth Settings menu.
 */

int latestID = 0;
AncsNotification notifications[8];

void setup() {
  // Serial port is initialized automatically; we don't have to do anything
  BeanAncs.enable();
}

void loop() {
  int msgAvail = BeanAncs.notificationsAvailable();
  if (msgAvail) {
    // blink red every time we have a new message
    Bean.setLedGreen(0);
    Bean.setLedRed(130);

    // fills the notifications array with message header info
    BeanAncs.getNotificationHeaders(notifications, 8);

    for (int i = 0; i < msgAvail; i++) {
      Serial.print("cat:");
      Serial.println(notifications[i].catID);
      Serial.print("flg:");
      Serial.println(notifications[i].flags);
      Serial.print("evt:");
      Serial.println(notifications[i].eventID);
      Serial.print("cnt:");
      Serial.println(notifications[i].catCount);
      Serial.print("id: ");
      Serial.println(notifications[i].notiUID);

      // get some information about each message
      char data[64] = {0};
      int len = BeanAncs.getNotificationAttributes(
        NOTI_ATTR_ID_MESSAGE,
        notifications[i].notiUID,
        39,
        (uint8_t *)data,
        5000);

      Serial.print("l: ");
      Serial.print(len);
      Serial.print(" ");
      for (int j = 0; j < len; j++) {
        Serial.print(data[j]);
      }
      Serial.println();
      delay(5000);
    }
    Bean.setLedRed(0);
  }

  // send a number through serial (with newline at end) to perform a negative
  // action on a message with the given ID
  if (Serial.available() > 0) {
    char idin[8] = {0};
    int index = 0;
    byte read = Serial.read();
    while (read != '\n' && index < 7) {
      idin[index] = read;
      index++;
      read = Serial.read();
    }

    int id = atoi(idin);
    Serial.print("Performing negative action on ID: ");
    Serial.println(id);
    BeanAncs.notificationAction(id, 0x01);
  }
}
