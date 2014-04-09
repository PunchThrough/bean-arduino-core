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

  uint16_t BeanClass::getAccelerationX(void){
    ACC_READING_T reading;
    if(Serial.accelRead(&reading) == 0){
      return reading.xAxis;
    }

    // TODO
    // this is an error state.  Is it worth a few retries?
    return 0;
  }

  uint16_t BeanClass::getAccelerationY(void){
    ACC_READING_T reading;
    if(Serial.accelRead(&reading) == 0){
      return reading.yAxis;
    }

    // TODO
    // this is an error state.  Is it worth a few retries?
    return 0;
  }


  uint16_t BeanClass::getAccelerationZ(void){
    ACC_READING_T reading;
    if(Serial.accelRead(&reading) == 0){
      return reading.zAxis;
    }

    // TODO
    // this is an error state.  Is it worth a few retries?
    return 0;
  }


  ACC_READING_T BeanClass::getAcceleration(void){
    ACC_READING_T reading;
    if(Serial.accelRead(&reading) == 0){
      return reading;
    }

    // TODO
    // this is an error state.  Is it worth a few retries?
    memset(&reading, 0, sizeof(reading));
    return reading;
  }

  void BeanClass::setLedRed(uint8_t intensity){
    LED_IND_SETTING_T setting;
    setting.color = (uint8_t) LED_RED;
    setting.intensity = intensity;

    Serial.ledSetSingle(setting);
  }

  void BeanClass::setLedGreen(uint8_t intensity){
    LED_IND_SETTING_T setting;
    setting.color = (uint8_t) LED_GREEN;
    setting.intensity = intensity;

    Serial.ledSetSingle(setting);
  }
    void BeanClass::setLedBlue(uint8_t intensity){
    LED_IND_SETTING_T setting;
    setting.color = (uint8_t) LED_BLUE;
    setting.intensity = intensity;

    Serial.ledSetSingle(setting);
  }

  void BeanClass::setLed(uint8_t red, uint8_t green, uint8_t blue){
    LED_SETTING_T setting = {red, green, blue};
    Serial.ledSet(setting);
  }

  uint8_t BeanClass::getLedRed(void){
    LED_SETTING_T reading;

    if(Serial.ledRead(&reading) == 0) {
      return reading.red;
    }

    // TODO: This is an error state.
    // Should we retry?
    return 0;
  }

  uint8_t BeanClass::getLedGreen(void){
    LED_SETTING_T reading;

    if(Serial.ledRead(&reading) == 0) {
      return reading.green;
    }

    // TODO: This is an error state.
    // Should we retry?
    return 0;
  }

  uint8_t BeanClass::getLedBlue(void){
    LED_SETTING_T reading;

    if(Serial.ledRead(&reading) == 0) {
      return reading.blue;
    }

    // TODO: This is an error state.
    // Should we retry?
    return 0;
  }


  LED_SETTING_T BeanClass::getLed(void){
    LED_SETTING_T reading;

    if(Serial.ledRead(&reading) == 0){
      return reading;
    }

    memset(&reading, 0, sizeof(reading));
    return reading;
  }
