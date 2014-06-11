
#ifndef BeanSerialTransport_h
#define BeanSerialTransport_h

#include "HardwareSerial.h"
#include "applicationMessageHeaders/AppMessages.h"

struct ScratchData {
  uint8_t length;
  uint8_t data[20];
};

// Used for waking the CC out of deep sleep mode.
#define CC_INTERRUPT_PIN (13)

class BeanSerialTransport : public HardwareSerial
{
  friend class BeanClass;

protected:
  ring_buffer *_reply_buffer;
  void insert_escaped_char(uint8_t input);
  volatile bool* _message_complete;


  size_t write_message(uint16_t messageId, const uint8_t* body, size_t body_length);

  int call_and_response(MSG_ID_T messageId,
                         const uint8_t *body,
                         size_t body_length,
                         uint8_t * response,
                         size_t * response_length, unsigned long timeout_ms=100);


// API Control
  //BT
  void BTSetAdvertisingOnOff(const bool setting);
  void BTSetAdvertisingInterval(const int interval_ms);
  void BTSetConnectionInterval(const int interval_ms);
  void BTSetLocalName(const char* name);
  void BTSetPairingPin(const uint16_t pin);
  void BTSetTxPower(const BT_TXPOWER_DB_T& power);
  void BTSetScratchChar(BT_SCRATCH_T *setting, uint8_t length);
  int  BTGetScratchChar(uint8_t scratchNum, ScratchData * scratchData);
  int  BTGetConfig(BT_RADIOCONFIG_T *config);

  //LED Control
  void ledSet(const LED_SETTING_T &setting);
  void ledSetSingle(const LED_IND_SETTING_T &setting);
  int ledRead(LED_SETTING_T *reading);

  //Accelerometer
  int accelRead(ACC_READING_T* reading);

  //temperature
  int temperatureRead( int8_t* tempRead );

  //battery
  int batteryRead( uint8_t* level );

  //Arduino Sleep
  void sleep(uint32_t duration_ms);


public:
  // To work on bean, the serial must be initialized
  // at 57600 with standard settings, and cannot be disabled
  // or all control messaging will break.  We've overidden begin() and end()
  // functions to not do a whole heck of a lot as a result.
  void begin(void);
  virtual void begin(unsigned long ignored){
    // Do nothing.
    // We're overiding what users can do here.
  }
  virtual void begin(unsigned long ignored, uint8_t also_ignored){
    // Do nothing.
    // We're overiding what users can do here.
  }
  virtual void end(){
    // Do nothing.  Users ending serial would break our control messaging
    // so we're removing this ability.
  }

  virtual void flush(void);

  virtual size_t write(uint8_t);
  size_t write(const uint8_t *buffer, size_t size);

  using HardwareSerial::write; // pull in write(str) and write(buf, size) from Print
  virtual size_t print(const String &s);
  size_t print(const __FlashStringHelper *ifsh);
  using Print::print;

  //Debug
  bool debugLoopbackVerify(const uint8_t *message, const size_t size);
  bool debugEndToEndLoopbackVerify(const uint8_t *message, const size_t size);
  int debugGetDebugCounter(int *counter);
  void debugWrite(const char c){ HardwareSerial::write((uint8_t)c);};
  void debugLoopBackFullSerialMessages(void);
  void debugWritePtm(const uint8_t *message, const size_t size);


  // constructor
    BeanSerialTransport(ring_buffer *rx_buffer, ring_buffer *tx_buffer,
                        volatile uint8_t *ubrrh, volatile uint8_t *ubrrl,
                        volatile uint8_t *ucsra, volatile uint8_t *ucsrb,
                        volatile uint8_t *ucsrc, volatile uint8_t *udr,
                        uint8_t rxen, uint8_t txen, uint8_t rxcie,
                        uint8_t udrie, uint8_t u2x, ring_buffer* reply_buffer,
                        volatile bool* message_complete)
                        : HardwareSerial(rx_buffer, tx_buffer, ubrrh, ubrrl,
                                         ucsra, ucsrb, ucsrc, udr, rxen, txen,
                                         rxcie, udrie, u2x)
  {
    _reply_buffer = reply_buffer;
    _message_complete = message_complete;
    *message_complete = false;
  }; // End constructor

}; // End BeanSerialTransport

#if defined(UBRRH) || defined(UBRR0H)
  extern BeanSerialTransport Serial;
#elif defined(USBCON)
  #include "USBAPI.h"
//  extern HardwareSerial Serial_;
#endif

#endif