#ifndef BEAN_BEAN_BEAN_H
#define BEAN_BEAN_BEAN_H
#include "BeanSerialTransport.h"

typedef BT_TXPOWER_DB_T BluetoothTransmitPowerType;
typedef ACC_AXIS_T AccelerometerAxisType;
typedef ACC_READING_T AccelerometerReadingType;
typedef LED_COLOR_T LedColorType;
typedef LED_SETTING_T LedSettingType;


class BeanClass {
public:
  void bleAdvertisingIntervalWrite(uint16_t internval_ms);
  void bleConnectionIntervalWrite(uint16_t interval_ms);
  unsigned int bleAdvertisingIntervalRead(void);
  unsigned int bleConnectionIntervalRead(void);

  void bleTxPowerWrite(BluetoothTransmitPowerType power);
  BluetoothTransmitPowerType bleTXPowerRead(void);

  uint16_t accelerometerAxisRead(AccelerometerAxisType axis);
  AccelerometerReadingType accelerometerRead(void);

  void ledIndividualColorWrite(LedColorType color, uint8_t intensity);
  void ledColorWrite(LedSettingType setting);
  uint8_t ledIndividualColorRead(LedColorType color);
  LedSettingType ledColorRead(void);

  BeanClass(){}
};

extern BeanClass Bean;


#endif