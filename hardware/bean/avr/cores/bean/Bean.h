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
  uint8_t getAccelerationRange (void);
  void setAccelerationRange (uint8_t range);

  int8_t getTemperature (void);
  uint8_t getBatteryLevel (void);
  uint16_t getBatteryVoltage (void);
  void enableConfigSave( bool enableSave );


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
  void keepAwake(bool enable);

  void attachChangeInterrupt(uint8_t pin, void(*userFunc)(void) );
  void detachChangeInterrupt(uint8_t pin);
  void setAdvertisingInterval( uint16_t interval_ms );
  void enableAdvertising(bool enable, uint32_t timer);
  void enableAdvertising(bool enable);
  bool getConnectionState( void );
  bool getAdvertisingState( void );
  void setBeanName( const String &s );
  const char* getBeanName(void);
  void setBeaconParameters( uint16_t uuid, uint16_t major_id, uint16_t minor_id );
  void setBeaconEnable( bool beaconEnable );
  void enableWakeOnConnect( bool enable );
  void disconnect(void);

  BeanClass(){}

private:
  bool attemptSleep( uint32_t duration_ms);

};

extern BeanClass Bean;


#endif
