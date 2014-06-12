#include "Bean.h"
#include "Arduino.h"
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include "wiring_private.h"

#ifndef sleep_bod_disable() // not included in Arduino AVR toolset
#define sleep_bod_disable() \
do { \
  uint8_t tempreg; \
  __asm__ __volatile__("in %[tempreg], %[mcucr]" "\n\t" \
                       "ori %[tempreg], %[bods_bodse]" "\n\t" \
                       "out %[mcucr], %[tempreg]" "\n\t" \
                       "andi %[tempreg], %[not_bodse]" "\n\t" \
                       "out %[mcucr], %[tempreg]" \
                       : [tempreg] "=&d" (tempreg) \
                       : [mcucr] "I" _SFR_IO_ADDR(MCUCR), \
                         [bods_bodse] "i" (_BV(BODS) | _BV(BODSE)), \
                         [not_bodse] "i" (~_BV(BODSE))); \
} while (0)
#endif


#define MAX_SCRATCH_SIZE (20)

  BeanClass Bean;

  static void wakeUp(void){
    // Do nothing.
    // This function is called as an interrupt purely to wake
    // us up.
    return;
  }

  void BeanClass::sleep(uint32_t duration_ms){

    // Send the sleep message to the TI and wait for it to
    // finish sending.
    Serial.sleep(duration_ms);
    Serial.flush();

    // set our interrupt pin to input:
    const int interruptNum = 1;

    bool adc_was_set = bit_is_set(ADCSRA, ADEN);
    if(adc_was_set){
      // disable ADC
      ADCSRA &= ~(_BV(ADEN));
    }

    bool ac_was_set = bit_is_set(ACSR, ACD);
    if(ac_was_set){
      // disable ADC
      ACSR &= ~(_BV(ACD));
    }


    // ensure that our interrupt line is an input
    DDRD &= ~(_BV(3));

    // Details on how to manage sleep mode with AVR gotten from the avr-libc
    // manual, found here: http://www.nongnu.org/avr-libc/user-manual/group__avr__sleep.html
    // (block quote below)

        // Note that unless your purpose is to completely lock the CPU (until a
        // hardware reset), interrupts need to be enabled before going to sleep.

        // As the sleep_mode() macro might cause race conditions in some situations,
        // the individual steps of manipulating the sleep enable (SE) bit, and
        // actually issuing the SLEEP instruction, are provided in the macros
        // sleep_enable(), sleep_disable(), and sleep_cpu(). This also allows for
        // test-and-sleep scenarios that take care of not missing the interrupt
        // that will awake the device from sleep.

        // Example:

        //     #include <avr/interrupt.h>
        //     #include <avr/sleep.h>

        //     ...
        //       set_sleep_mode(<mode>);
        //       cli();
        //       if (some_condition)
        //       {
        //         sleep_enable();
        //         sei();
        //         sleep_cpu();
        //         sleep_disable();
        //       }
        //       sei();
        // This sequence ensures an atomic test of some_condition with interrupts
        // being disabled. If the condition is met, sleep mode will be prepared,
        // and the SLEEP instruction will be scheduled immediately after an SEI
        // instruction. As the intruction right after the SEI is guaranteed to be
        // executed before an interrupt could trigger, it is sure the device will
        // really be put to sleep.

        // Some devices have the ability to disable the Brown Out Detector (BOD)
        // before going to sleep. This will also reduce power while sleeping.
        // If the specific AVR device has this ability then an additional macro
        // is defined: sleep_bod_disable(). This macro generates inlined assembly
        // code that will correctly implement the timed sequence for disabling the
        // BOD before sleeping. However, there is a limited number of cycles after
        // the BOD has been disabled that the device can be put into sleep mode,
        // otherwise the BOD will not truly be disabled. Recommended practice is
        // to disable the BOD (sleep_bod_disable()), set the interrupts (sei()),
        // and then put the device to sleep (sleep_cpu()), like so:

        //     #include <avr/interrupt.h>
        //     #include <avr/sleep.h>

        //     ...
        //       set_sleep_mode(<mode>);
        //       cli();
        //       if (some_condition)
        //       {
        //         sleep_enable();
        //         sleep_bod_disable();
        //         sei();
        //         sleep_cpu();
        //         sleep_disable();
        //       }
        //       sei();


     /* In the function call attachInterrupt(A, B, C)
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
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    cli();
    if (bit_is_set(PIND, 3))
    {
      sleep_enable();
      sleep_bod_disable();
      sei();
      sleep_cpu();
      sleep_disable();
    }
    sei();

    detachInterrupt(interruptNum);

    if(adc_was_set){
      //re-enable adc
      ADCSRA |= _BV(ADEN);
    }

    if(ac_was_set){
      //re-enable analog compareter
      ACSR |= _BV(ACD);
    }
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

int8_t BeanClass::getTemperature(void)
{
  int8_t temp = 0;

  Serial.temperatureRead(&temp);

  return temp;
}

uint8_t BeanClass::getBatteryLevel(void)
{
  uint8_t level = 0;

  Serial.batteryRead(&level);

  return level;
}

uint16_t BeanClass::getBatteryVoltage(void)
{
  uint32_t actualVoltage = 0;
  uint8_t level = 0;

  Serial.batteryRead(&level);

  // This may not return accurate readings.  Conversion is subject to change.
  // The conversion function from voltage to level is as follows:
  //  f(x) = x * (63.53) - 124.26
  //
  //  solve for voltage and you get the function below, including fixed-point scaling
  //  with two decimal points of precision
  actualVoltage = (((uint32_t)100 * (uint32_t)level + (uint32_t)12426 ) * (uint32_t)100 ) / 6353;

  return (uint16_t)actualVoltage;
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

  bool BeanClass::setScratchData(uint8_t bank, const uint8_t* data, uint8_t dataLength)
  {
    bool errorRtn = true;

    if ( dataLength <= MAX_SCRATCH_SIZE )
    {
      BT_SCRATCH_T scratch;
      scratch.number = bank;
      memcpy( (void*)scratch.scratch, (void*)data, dataLength);
      // magic: +1 due to bank byte
      Serial.BTSetScratchChar(&scratch, (uint8_t)(dataLength+1));
    }
    else
    {
      errorRtn = false;
    }

    return errorRtn;
  }

  bool BeanClass::setScratchNumber(uint8_t bank, uint32_t data)
  {
    bool errorRtn = true;
    BT_SCRATCH_T scratch;
    scratch.number = bank;

    scratch.scratch[0] = data & 0xFF;
    scratch.scratch[1] = data >> 8UL;
    scratch.scratch[2] = data >> 16UL;
    scratch.scratch[3] = data >> 24UL;

    // magic: 4 for data, 1 for scratch bank
    Serial.BTSetScratchChar(&scratch, 4+1);

    return errorRtn;
  }

  ScratchData BeanClass::readScratchData(uint8_t bank)
  {
    ScratchData scratchTempBuffer;

    memset( scratchTempBuffer.data, 0, 20);
    Serial.BTGetScratchChar( bank, &scratchTempBuffer );

    return scratchTempBuffer;
  }

  long BeanClass::readScratchNumber(uint8_t bank)
  {
    long returnNum = 0;
    static ScratchData scratchNumBuffer;

    memset( scratchNumBuffer.data, 0, 20);
    Serial.BTGetScratchChar( bank, &scratchNumBuffer );

    returnNum |= (long)scratchNumBuffer.data[0] & 0xFF;
    returnNum |= (long)scratchNumBuffer.data[1] << 8UL;
    returnNum |= (long)scratchNumBuffer.data[2] << 16UL;
    returnNum |= (long)scratchNumBuffer.data[3] << 24UL;

    return returnNum;
  }