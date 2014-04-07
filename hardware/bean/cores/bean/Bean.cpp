#include "Bean.h"

  BeanClass Bean;


  void BeanClass::bleAdvertisingIntervalWrite(uint16_t interval_ms){
    Serial.BTSetAdvertisingInterval(interval_ms);
  }

  void BeanClass::bleConnectionIntervalWrite(uint16_t interval_ms){
    Serial.BTSetAdvertisingInterval(interval_ms);
  }

  unsigned int BeanClass::bleAdvertisingIntervalRead(void){
    BT_RADIOCONFIG_T config;

    if(Serial.BTGetConfig(&config) == 0){
      return config.adv_int;
    }

    // TODO
    // this is an error state.  Is it worth a few retries?
    return 0;
  }

  unsigned int BeanClass::bleConnectionIntervalRead(void){
    BT_RADIOCONFIG_T config;

    if(Serial.BTGetConfig(&config) == 0){
      return config.conn_int;
    }

    // TODO
    // this is an error state.  Is it worth a few retries?
    return 0;
  }

  void BeanClass::bleTxPowerWrite(BT_TXPOWER_DB_T power){
    Serial.BTSetTxPower(power);
  }

  BT_TXPOWER_DB_T BeanClass::bleTXPowerRead(void){
    BT_RADIOCONFIG_T config;

    if(Serial.BTGetConfig(&config) == 0){
      return (BT_TXPOWER_DB_T)config.power;
    }

    // TODO
    // this is an error state.  Is it worth a few retries?
    return TXPOWER_4DB;
  }

  uint16_t BeanClass::accelerometerAxisRead(ACC_AXIS_T axis){
    ACC_READING_T reading;
    if(Serial.accelRead(&reading) == 0){
      switch(axis){
        case ACC_AXIS_X:
          return reading.xAxis;
        break;

        case ACC_AXIS_Y:
          return reading.yAxis;
        break;

        case ACC_AXIS_Z:
          return reading.zAxis;
        break;
      }
    }

    // TODO
    // this is an error state.  Is it worth a few retries?
    return 0;
  }

  ACC_READING_T BeanClass::accelerometerRead(void){
    ACC_READING_T reading;
    if(Serial.accelRead(&reading) == 0){
      return reading;
    }

    // TODO
    // this is an error state.  Is it worth a few retries?
    memset(&reading, 0, sizeof(reading));
    return reading;
  }

  void BeanClass::ledIndividualColorWrite(LED_COLOR_T color, uint8_t intensity){
    LED_IND_SETTING_T setting;
    setting.color = (uint8_t) color;
    setting.intensity = intensity;

    Serial.ledSetSingle(setting);
  }

  void BeanClass::ledColorWrite(LED_SETTING_T setting){
    Serial.ledSet(setting);
  }
  
  uint8_t BeanClass::ledIndividualColorRead(LED_COLOR_T color){
    LED_SETTING_T reading;

    if(Serial.ledRead(&reading) == 0){
      switch(color){
        case LED_RED:
          return reading.red;
        break;

        case LED_GREEN:
          return reading.green;
        break;

        case LED_BLUE:
          return reading.blue;
        break;
      }
    }

    return 0;
  }
  
  LED_SETTING_T BeanClass::ledColorRead(void){
    LED_SETTING_T reading;

    if(Serial.ledRead(&reading) == 0){
      return reading;
    }
  
    memset(&reading, 0, sizeof(reading));
    return reading;
  }



