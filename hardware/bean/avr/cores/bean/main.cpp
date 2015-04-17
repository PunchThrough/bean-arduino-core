#include <Arduino.h>

int main(void)
{
  init();

#if defined(USBCON)
  USBDevice.attach();
#endif
  // Ensure that BeanSerialTransport.begin() is called for control messages
  // even if users are not using the serial port.
  // A user calling this again shouldn't cause any harm.

  // Need to turn off SPI as it's on at boot for some reason
  SPCR &= ~_BV(SPE);

  Serial.begin();
  setup();

  for (;;) {
    loop();
    if (serialEventRun) serialEventRun();
  }

  return 0;
}

