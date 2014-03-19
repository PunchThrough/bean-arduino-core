
#ifndef BeanSerialTransport_h
#define BeanSerialTransport_h

#include "HardwareSerial.h"


class BeanSerialTransport : public HardwareSerial
{

protected:
  ring_buffer *_reply_buffer;
  void insert_escaped_char(uint8_t input);
  volatile bool* _message_complete;


  size_t write_message(uint16_t messageId, const uint8_t* body, size_t body_length);

  void call_and_response(uint16_t messageId, 
                         const uint8_t *body,
                         size_t body_length,
                         uint8_t * response,
                         size_t & response_length);


public:
  virtual size_t write(uint8_t);
  size_t write(const uint8_t *buffer, size_t size);
  void debug_write(const char c){ HardwareSerial::write((uint8_t)c);};

  using HardwareSerial::write; // pull in write(str) and write(buf, size) from Print
  virtual size_t print(const String &s);
  size_t print(const __FlashStringHelper *ifsh);
  using Print::print;

// Radio Control
  typedef enum {
     TxPower_4dB = 0,
     TxPower_0dB,
     TxPower_neg6dB,
     TxPower_neg23dB,
  } TxPower_dB;

  void setAdvertisingInterval(int interval_ms);
  int advertisingInterval(void);

  void setTxPower(TxPower_dB power);
  TxPower_dB txPower(void);

// Ardiono Power Control
  void setAtmegaPowerOnInterval(int interval_ms);
  int atmegaPowerOnInterval(void);
  void powerOff(void);

// Accelerometer
  typedef enum {
    AccelerometerAxisX = 0,
    AccelerometerAxisY,
    AccelerometerAxisZ,
  } AccelerometerAxis;
  
  uint16_t accelerometerAxis(AccelerometerAxis axis);



// LED Control
    typedef struct {
      uint8_t red;
      uint8_t green;
      uint8_t blue;
      uint8_t intensity;
    } BeanLedSetting;

    void setLed(BeanLedSetting &setting);
    BeanLedSetting readLed(void);


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