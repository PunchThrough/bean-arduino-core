#include "bma250.h"

#define POLL_RATE   100
uint16_t count;

void setup() {
  // put your setup code here, to run once:
  Serial.begin();
  Bean.accelerometerConfig(ENABLE_DOUBLE_TAP_INT | ENABLE_LOW_G_INT | ENABLE_ANY_MOTION_INT, VALUE_LOW_POWER_10MS);
  count = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  Bean.sleep(POLL_RATE);
  uint8_t status = Bean.checkAccelInterrupts();
  if (status & CHECK_DOUBLE_TAP_INT) {
    Bean.setLed(0xFF, 0x00, 0x00);
    count = 3000 / POLL_RATE;  // Hold for 5 seconds
  }
  else if (status & CHECK_LOW_G_INT) {
    Bean.setLed(0x00, 0xFF, 0x00);
    count = 3000 / POLL_RATE;  // Hold for 5 seconds
  }
  else if ((status & CHECK_ANY_MOTION_INT) && (count == 0)) {
    Bean.setLed(0x00, 0x00, 0xFF);
  }
  else if (count == 0) {
    Bean.setLed(0x00, 0x00, 0x00);
  }
  else if (count > 0) {
    count--;
  }
}
