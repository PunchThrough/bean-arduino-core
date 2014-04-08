#include "Bean.h"
#include "Arduino.h"
#include <avr/sleep.h>

  BeanClass Bean;

  static void wakeUp(void){
    // Do nothing.
    // This function is called as an intterupt purely to wake
    // us up.
    return;
  }

  void BeanClass::sleep(uint32_t duration_ms){

    // TODO send message asking to be woken up in duration_ms
    // set our interrupt pin to input:
    const int interruptNum = 1;

    /* Now is the time to set the sleep mode. In the Atmega8 datasheet
     * http://www.atmel.com/dyn/resources/prod_documents/doc2486.pdf on page 35
     * there is a list of sleep modes which explains which clocks and
     * wake up sources are available in which sleep mode.
     *
     * In the avr/sleep.h file, the call names of these sleep modes are to be found:
     *
     * The 5 different modes are:
     *     SLEEP_MODE_IDLE         -the least power savings
     *     SLEEP_MODE_ADC
     *     SLEEP_MODE_PWR_SAVE
     *     SLEEP_MODE_STANDBY
     *     SLEEP_MODE_PWR_DOWN     -the most power savings
     *
     * For now, we want as much power savings as possible, so we
     * choose the according
     * sleep mode: SLEEP_MODE_PWR_DOWN
     *
     */
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here
    sleep_enable();          // enables the sleep bit in the mcucr register
                             // so sleep is possible. just a safety pin

    /* Now it is time to enable an interrupt. We do it here so an
     * accidentally pushed interrupt button doesn't interrupt
     * our running program. if you want to be able to run
     * interrupt code besides the sleep function, place it in
     * setup() for example.
     *
     * In the function call attachInterrupt(A, B, C)
     * A   can be either 0 or 1 for interrupts on pin 2 or 3.
     *
     * B   Name of a function you want to execute at interrupt for A.
     *
     * C   Trigger mode of the interrupt pin. can be:
     *             LOW        a low level triggers
     *             CHANGE     a change in level triggers
     *             RISING     a rising edge of a level triggers
     *             FALLING    a falling edge of a level triggers
     *
     * In all but the IDLE sleep modes only LOW can be used.
     */

    attachInterrupt(interruptNum,wakeUp, LOW);
    sleep_mode();            // here the device is actually put to sleep!!

    // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP

    sleep_disable();
    detachInterrupt(interruptNum);
  }

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
