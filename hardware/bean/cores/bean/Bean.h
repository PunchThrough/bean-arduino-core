#ifndef BEAN_BEAN_BEAN_H
#define BEAN_BEAN_BEAN_H
#include "BeanSerialTransport.h"

class BeanClass {
public:
  void setBleAdvertisingInterval(uint16_t internval_ms);
  void setBleConnectionInterval(uint16_t interval_ms);
  unsigned int bleAdvertisingInterval(void);
  unsigned int bleConnectionInterval(void);

  void setBleTxPower(BT_TXPOWER_DB_T power);
  BT_TXPOWER_DB_T bleTXPower(void);

  uint16_t accelerometerAxis(ACC_AXIS_T axis);
  ACC_READING_T accelerometerReading(void);

  void setLedIndividualColor(LED_COLOR_T color, uint8_t intensity);
  void setLedColor(LED_SETTING_T setting);
  uint8_t getLedIndividualColor(LED_COLOR_T color);
  LED_SETTING_T ledColor(void);

  BeanClass(){}
};

extern BeanClass Bean;


#endif