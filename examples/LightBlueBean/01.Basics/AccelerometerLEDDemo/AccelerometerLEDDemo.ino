#include "bma250.h"

#define POLL_RATE         3000
#define SUPAH_COUNTDOWN   5

uint16_t count;
uint16_t supah_sleep;

void setup() {
  // put your setup code here, to run once:
  Serial.begin();
  Bean.accelerometerConfig(ENABLE_DOUBLE_TAP_INT | ENABLE_LOW_G_INT | ENABLE_ANY_MOTION_INT, VALUE_LOW_POWER_10MS);
  Bean.enableWakeOnAccelerometer(WAKE_LOW_G_INT | WAKE_DOUBLE_TAP_INT);
  count = 0;
  supah_sleep = 10;
}

void loop() {
  // put your main code here, to run repeatedly:
  Bean.sleep(POLL_RATE);
  uint8_t status = Bean.checkAccelInterrupts();
  if (status > 0) {
    supah_sleep = SUPAH_COUNTDOWN;
    Bean.enableAdvertising(true);
  }
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
    if (supah_sleep == 0) {
      Bean.enableAdvertising(false);  // Disable advertising
      Bean.setLed(0xFF, 0xFF, 0xFF);  // Flash the crowd
      Bean.sleep(500);              
      Bean.setLed(0x00, 0x00, 0x00);
      Bean.sleep(60000);              // Sleep for a minute
    }
    else {
      supah_sleep--;
      Bean.setLed(0x00, 0x00, 0x00);
    }
  }
  else if (count > 0) {
    count--;
  }
}

