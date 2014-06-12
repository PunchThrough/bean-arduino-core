#ifndef BEAN_BEAN_BEAN_H
#define BEAN_BEAN_BEAN_H
#include "BeanSerialTransport.h"

typedef ACC_READING_T AccelerationReading;
typedef LED_SETTING_T LedReading;

class BeanClass {
public:
  uint16_t getAccelerationX (void);
  uint16_t getAccelerationY (void);
  uint16_t getAccelerationZ (void);
  AccelerationReading getAcceleration (void);

  int8_t getTemperature (void);
  uint8_t getBatteryLevel (void);
  uint16_t getBatteryVoltage (void);

  void setLed(uint8_t red, uint8_t green, uint8_t blue);
  LedReading getLed(void);
  uint8_t getLedRed (void);
  uint8_t getLedGreen (void);
  uint8_t getLedBlue (void);
  void setLedRed(uint8_t intensity);
  void setLedGreen(uint8_t intensity);
  void setLedBlue(uint8_t intensity);

  bool setScratchData(uint8_t bank, const uint8_t* data, uint8_t dataLength);
  bool setScratchNumber(uint8_t bank, uint32_t data);
  ScratchData readScratchData(uint8_t bank);
  long readScratchNumber(uint8_t bank);

  void sleep(uint32_t duration_ms);

  BeanClass(){}
};

extern BeanClass Bean;


#endif
