

/* Copyright (c) 2011, Peter Barrett
**
** Permission to use, copy, modify, and/or distribute this software for
** any purpose with or without fee is hereby granted, provided that the
** above copyright notice and this permission notice appear in all copies.
**
** THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
** WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR
** BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES
** OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
** WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
** ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
** SOFTWARE.
*/

#include <Arduino.h>
#include "BeanHID.h"

// Singleton
BeanHid_ BeanHid;

#define HID_DEV_DATA_LEN 8

//==============================================================================
//==============================================================================

// HID report descriptor

#define LSB(_x) ((_x)&0xFF)
#define MSB(_x) ((_x) >> 8)

#define HID_RPT_ID_KEY_IN 2    // Keyboard input report ID
#define HID_RPT_ID_MOUSE_IN 1  // Mouse input report ID
#define HID_RPT_ID_CC_IN 3     // Consumer Control input report ID

#define HID_RPT_ID_LED_OUT 0  // LED output report ID
#define HID_RPT_ID_FEATURE 0  // Feature report ID

#define HID_REPORT_TYPE_INPUT 1
#define HID_REPORT_TYPE_OUTPUT 2
#define HID_REPORT_TYPE_FEATURE 3

typedef struct {
  uint8_t id;
  uint8_t type;
  uint8_t len;
  uint8_t data[HID_DEV_DATA_LEN];
} hidDevReport_t;

// HID Consumer Control keycodes (based on the HID Report Map characteristic
// value)
#define HID_CC_RPT_MUTE 1
#define HID_CC_RPT_POWER 2
#define HID_CC_RPT_LAST 3
#define HID_CC_RPT_ASSIGN_SEL 4
#define HID_CC_RPT_PLAY 5
#define HID_CC_RPT_PAUSE 6
#define HID_CC_RPT_RECORD 7
#define HID_CC_RPT_FAST_FWD 8
#define HID_CC_RPT_REWIND 9
#define HID_CC_RPT_SCAN_NEXT_TRK 10
#define HID_CC_RPT_SCAN_PREV_TRK 11
#define HID_CC_RPT_STOP 12

#define HID_CC_RPT_CHANNEL_UP 0x01
#define HID_CC_RPT_CHANNEL_DOWN 0x03
#define HID_CC_RPT_VOLUME_UP 0x40
#define HID_CC_RPT_VOLUME_DOWN 0x80

// HID Consumer Control report bitmasks
#define HID_CC_RPT_NUMERIC_BITS 0xF0
#define HID_CC_RPT_CHANNEL_BITS 0xCF
#define HID_CC_RPT_VOLUME_BITS 0x3F
#define HID_CC_RPT_BUTTON_BITS 0xF0
#define HID_CC_RPT_SELECTION_BITS 0xCF

// Num pad
#define HID_CC_RPT_KEYBOARD_1 30  // 0x1E - Keyboard 1 and !
// Other keys are mapped between these two...
#define HID_CC_RPT_KEYBOARD_0 39  // 0x27 - Keyboard 0 and )

// Macros for the HID Consumer Control 2-byte report
#define HID_CC_RPT_SET_NUMERIC(s, x) \
  (s)[0] &= HID_CC_RPT_NUMERIC_BITS; \
  (s)[0] = (x)
#define HID_CC_RPT_SET_CHANNEL(s, x) \
  (s)[0] &= HID_CC_RPT_CHANNEL_BITS; \
  (s)[0] |= ((x)&0x03) << 4
#define HID_CC_RPT_SET_VOLUME_UP(s) \
  (s)[0] &= HID_CC_RPT_VOLUME_BITS; \
  (s)[0] |= 0x40
#define HID_CC_RPT_SET_VOLUME_DOWN(s) \
  (s)[0] &= HID_CC_RPT_VOLUME_BITS;   \
  (s)[0] |= 0x80
#define HID_CC_RPT_SET_BUTTON(s, x) \
  (s)[1] &= HID_CC_RPT_BUTTON_BITS; \
  (s)[1] |= (x)
#define HID_CC_RPT_SET_SELECTION(s, x) \
  (s)[1] &= HID_CC_RPT_SELECTION_BITS; \
  (s)[1] |= ((x)&0x03) << 4

extern const uint8_t _asciimap[128] PROGMEM;

#define SHIFT 0x80
const uint8_t _asciimap[128] = {
    0x00,  // NUL
    0x00,  // SOH
    0x00,  // STX
    0x00,  // ETX
    0x00,  // EOT
    0x00,  // ENQ
    0x00,  // ACK
    0x00,  // BEL
    0x2a,  // BS Backspace
    0x2b,  // TAB Tab
    0x28,  // LF Enter
    0x00,  // VT
    0x00,  // FF
    0x00,  // CR
    0x00,  // SO
    0x00,  // SI
    0x00,  // DEL
    0x00,  // DC1
    0x00,  // DC2
    0x00,  // DC3
    0x00,  // DC4
    0x00,  // NAK
    0x00,  // SYN
    0x00,  // ETB
    0x00,  // CAN
    0x00,  // EM
    0x00,  // SUB
    0x00,  // ESC
    0x00,  // FS
    0x00,  // GS
    0x00,  // RS
    0x00,  // US

    0x2c,          //  ' '
    0x1e | SHIFT,  // !
    0x34 | SHIFT,  // "
    0x20 | SHIFT,  // #
    0x21 | SHIFT,  // $
    0x22 | SHIFT,  // %
    0x24 | SHIFT,  // &
    0x34,          // '
    0x26 | SHIFT,  // (
    0x27 | SHIFT,  // )
    0x25 | SHIFT,  // *
    0x2e | SHIFT,  // +
    0x36,          // ,
    0x2d,          // -
    0x37,          // .
    0x38,          // /
    0x27,          // 0
    0x1e,          // 1
    0x1f,          // 2
    0x20,          // 3
    0x21,          // 4
    0x22,          // 5
    0x23,          // 6
    0x24,          // 7
    0x25,          // 8
    0x26,          // 9
    0x33 | SHIFT,  // :
    0x33,          // ;
    0x36 | SHIFT,  // <
    0x2e,          // =
    0x37 | SHIFT,  // >
    0x38 | SHIFT,  // ?
    0x1f | SHIFT,  // @
    0x04 | SHIFT,  // A
    0x05 | SHIFT,  // B
    0x06 | SHIFT,  // C
    0x07 | SHIFT,  // D
    0x08 | SHIFT,  // E
    0x09 | SHIFT,  // F
    0x0a | SHIFT,  // G
    0x0b | SHIFT,  // H
    0x0c | SHIFT,  // I
    0x0d | SHIFT,  // J
    0x0e | SHIFT,  // K
    0x0f | SHIFT,  // L
    0x10 | SHIFT,  // M
    0x11 | SHIFT,  // N
    0x12 | SHIFT,  // O
    0x13 | SHIFT,  // P
    0x14 | SHIFT,  // Q
    0x15 | SHIFT,  // R
    0x16 | SHIFT,  // S
    0x17 | SHIFT,  // T
    0x18 | SHIFT,  // U
    0x19 | SHIFT,  // V
    0x1a | SHIFT,  // W
    0x1b | SHIFT,  // X
    0x1c | SHIFT,  // Y
    0x1d | SHIFT,  // Z
    0x2f,          // [
    0x31,          // bslash
    0x30,          // ]
    0x23 | SHIFT,  // ^
    0x2d | SHIFT,  // _
    0x35,          // `
    0x04,          // a
    0x05,          // b
    0x06,          // c
    0x07,          // d
    0x08,          // e
    0x09,          // f
    0x0a,          // g
    0x0b,          // h
    0x0c,          // i
    0x0d,          // j
    0x0e,          // k
    0x0f,          // l
    0x10,          // m
    0x11,          // n
    0x12,          // o
    0x13,          // p
    0x14,          // q
    0x15,          // r
    0x16,          // s
    0x17,          // t
    0x18,          // u
    0x19,          // v
    0x1a,          // w
    0x1b,          // x
    0x1c,          // y
    0x1d,          // z
    0x2f | SHIFT,  //
    0x31 | SHIFT,  // |
    0x30 | SHIFT,  // }
    0x35 | SHIFT,  // ~
    0              // DEL
};

// Mouse

uint8_t _buttons;
void buttons(uint8_t b);
void sendReport(MouseReport *commands);

// Keyboard

KeyReport _keyReport;
CcReport ccReportForHeldCommands = {0, 0};
void sendReport(KeyReport *keys);

//==============================================================================
//==============================================================================
// Driver

uint8_t _hid_idle = 1;

//==============================================================================
//==============================================================================
// BeanHid

// Private functions
// void addCommandToCcReport(CcReport * pReport, uint8_t cmd);
// void _genericSendReport(uint8_t id, uint8_t *buffer, size_t length);
// void sendReport(MouseReport *pReport);
// void sendReport(KeyReport *pReport);
// void sendReport(CcReport *pReport);
// void buttons(uint8_t b);

BeanHid_::BeanHid_(void) {
  _buttons = 0;
  isShiftHeld = false;
}

// Private functions

static void addCommandToCcReport(CcReport *pReport, uint8_t cmd) {
  /*
    Byte 0 - 4 LSB(bits 0,1,2,3) are used by Keypad

    Byte 0 - bits 4 & 5 are used for Channel Up/Down

    Byte 0 - bits 6 & 7 are used for Volume Up/Down

    Byte 1 - 4 LSB(bits 0,1,2,3) are used for Media keys(Play,Pause, mute etc..)

    Byte 1 - bits 4 & 5 are used for Button state information.

    Byte 1 - bits 6 & 7 are not defined in report map and hence ignored by host.
  */

  switch (cmd) {
    case CHANNEL_UP:
      HID_CC_RPT_SET_CHANNEL(pReport->bytes, HID_CC_RPT_CHANNEL_UP);
      break;

    case CHANNEL_DOWN:
      HID_CC_RPT_SET_CHANNEL(pReport->bytes, HID_CC_RPT_CHANNEL_DOWN);
      break;

    case VOLUME_UP:
      HID_CC_RPT_SET_VOLUME_UP(pReport->bytes);
      break;

    case VOLUME_DOWN:
      HID_CC_RPT_SET_VOLUME_DOWN(pReport->bytes);
      break;

    case MUTE:
      HID_CC_RPT_SET_BUTTON(pReport->bytes, HID_CC_RPT_MUTE);
      break;

    case POWER:
      HID_CC_RPT_SET_BUTTON(pReport->bytes, HID_CC_RPT_POWER);
      break;

    case RECALL_LAST:
      HID_CC_RPT_SET_BUTTON(pReport->bytes, HID_CC_RPT_LAST);
      break;

    case ASSIGN_SEL:
      HID_CC_RPT_SET_BUTTON(pReport->bytes, HID_CC_RPT_ASSIGN_SEL);
      break;

    case PLAY:
      HID_CC_RPT_SET_BUTTON(pReport->bytes, HID_CC_RPT_PLAY);
      break;

    case PAUSE:
      HID_CC_RPT_SET_BUTTON(pReport->bytes, HID_CC_RPT_PAUSE);
      break;

    case RECORD:
      HID_CC_RPT_SET_BUTTON(pReport->bytes, HID_CC_RPT_RECORD);
      break;

    case FAST_FORWARD:
      HID_CC_RPT_SET_BUTTON(pReport->bytes, HID_CC_RPT_FAST_FWD);
      break;

    case REWIND:
      HID_CC_RPT_SET_BUTTON(pReport->bytes, HID_CC_RPT_REWIND);
      break;

    case SCAN_NEXT_TRK:
      HID_CC_RPT_SET_BUTTON(pReport->bytes, HID_CC_RPT_SCAN_NEXT_TRK);
      break;

    case SCAN_PREV_TRK:
      HID_CC_RPT_SET_BUTTON(pReport->bytes, HID_CC_RPT_SCAN_PREV_TRK);
      break;

    case STOP:
      HID_CC_RPT_SET_BUTTON(pReport->bytes, HID_CC_RPT_STOP);
      break;

    default:
      if ((cmd >= HID_CC_RPT_KEYBOARD_1) && (cmd <= HID_CC_RPT_KEYBOARD_0)) {
        HID_CC_RPT_SET_BUTTON(pReport->bytes,
                              (cmd - HID_CC_RPT_KEYBOARD_1 + 1) % 10);
      }
      break;
  }
}

void BeanHid_::_genericSendReport(uint8_t id, uint8_t *buffer, size_t length) {
  hidDevReport_t report;
  report.type = HID_REPORT_TYPE_INPUT;
  report.id = id;
  report.len = length;

  if (report.len <= HID_DEV_DATA_LEN) {
    memcpy((void *)report.data, (void *)buffer, length);
    Serial.write_message(MSG_ID_HID_SEND_REPORT, (uint8_t *)&report,
                         sizeof(hidDevReport_t));
  }
}

void BeanHid_::sendReport(MouseReport *pReport) {
  _genericSendReport(HID_RPT_ID_MOUSE_IN, (uint8_t *)pReport,
                     sizeof(MouseReport));
}

void BeanHid_::sendReport(KeyReport *pReport) {
  _genericSendReport(HID_RPT_ID_KEY_IN, (uint8_t *)pReport, sizeof(KeyReport));
}

void BeanHid_::sendReport(CcReport *pReport) {
  _genericSendReport(HID_RPT_ID_CC_IN, (uint8_t *)pReport, sizeof(CcReport));
}

void BeanHid_::buttons(uint8_t b) {
  if (b != _buttons) {
    _buttons = b;
    moveMouse(0, 0, 0);
  }
}

// Public functions

void BeanHid_::enable(void) {
  ADV_SWITCH_ENABLED_T curServices = Bean.getServices();
  curServices.hid = 1;
  Bean.setServices(curServices);
}

bool BeanHid_::isEnabled(void) {
  ADV_SWITCH_ENABLED_T curServices = Bean.getServices();
  return (curServices.hid == 1) ? true : false;
}

void BeanHid_::disable(void) {
  ADV_SWITCH_ENABLED_T curServices = Bean.getServices();
  curServices.hid = 0;
  Bean.setServices(curServices);
}

// Mouse
void BeanHid_::moveMouse(signed char delta_x, signed char delta_y,
                         signed char delta_wheel) {
  MouseReport m;
  m.mouse[0] = _buttons;
  m.mouse[1] = delta_x;
  m.mouse[2] = delta_y;
  m.mouse[3] = delta_wheel;
  sendReport(&m);
}

void BeanHid_::holdMouseClick(mouseButtons button) {
  buttons(_buttons | button);
}

void BeanHid_::releaseMouseClick(mouseButtons button) {
  buttons(_buttons & ~button);
}

void BeanHid_::sendMouseClick(mouseButtons button) {
  holdMouseClick(button);
  releaseMouseClick(button);
}

// Consumer Control
void BeanHid_::sendMediaControl(mediaControl command) {
  holdMediaControl(command);
  releaseMediaControl(command);
}

void BeanHid_::holdMediaControl(mediaControl command) {
  addCommandToCcReport(&ccReportForHeldCommands, command);
  sendReport(&ccReportForHeldCommands);
}

void BeanHid_::releaseMediaControl(mediaControl command) {
  CcReport report = {0, 0};
  addCommandToCcReport(&report, command);

  // subtract "report" from our "held" report
  (ccReportForHeldCommands.bytes)[0] &= ~((report.bytes)[0]);
  (ccReportForHeldCommands.bytes)[1] &= ~((report.bytes)[1]);

  sendReport(&ccReportForHeldCommands);
}

void BeanHid_::releaseAllMediaControls() {
  ccReportForHeldCommands = {0, 0};
  sendReport(&ccReportForHeldCommands);
}

// Keyboard

// _holdKey() adds the specified key (printing, non-printing, or modifier)
// to the persistent key report and sends the report.  Because of the way
// USB HID works, the host acts like the key remains pressed until we
// call release(), releaseAll(), or otherwise clear the report and resend.
size_t BeanHid_::_holdKey(uint8_t k) {
  uint8_t i;
  if (k >= 136) {  // it's a non-printing key (not a modifier)
    k = k - 136;
  } else if (k >= 128) {  // it's a modifier key
    _keyReport.modifiers |= (1 << (k - 128));
    if (k == KEY_LEFT_SHIFT) { isShiftHeld = true; }
    k = 0;
  } else {  // it's a printing key
    k = pgm_read_byte(_asciimap + k);
    if (!k) {
      // setWriteError();
      return 0;
    }
    if (k &
        0x80) {  // it's a capital letter or other character reached with shift
      _keyReport.modifiers |= 0x02;  // the left shift modifier
      k &= 0x7F;
    }
  }

  // Add k to the key report only if it's not already present
  // and if there is an empty slot.
  if (_keyReport.keys[0] != k && _keyReport.keys[1] != k &&
      _keyReport.keys[2] != k && _keyReport.keys[3] != k &&
      _keyReport.keys[4] != k && _keyReport.keys[5] != k) {
    for (i = 0; i < 6; i++) {
      if (_keyReport.keys[i] == 0x00) {
        _keyReport.keys[i] = k;
        break;
      }
    }
    if (i == 6) {
      // setWriteError();
      return 0;
    }
  }
  sendReport(&_keyReport);
  return 1;
}

// _release() takes the specified key out of the persistent key report and
// sends the report.  This tells the OS the key is no longer pressed and that
// it shouldn't be repeated any more.
size_t BeanHid_::_releaseKey(uint8_t k) {
  uint8_t i;
  if (k >= 136) {  // it's a non-printing key (not a modifier)
    k = k - 136;
  } else if (k >= 128) {  // it's a modifier key
    _keyReport.modifiers &= ~(1 << (k - 128));
    if (k == KEY_LEFT_SHIFT) { isShiftHeld = false; }
    k = 0;
  } else {  // it's a printing key
    k = pgm_read_byte(_asciimap + k);
    if (!k) {
      return 0;
    }
    if (k &
        0x80) {  // it's a capital letter or other character reached with shift
      if (!isShiftHeld) { _keyReport.modifiers &= ~(0x02); }  // the left shift modifier
      k &= 0x7F;
    }
  }

  // Test the key report to see if k is present.  Clear it if it exists.
  // Check all positions in case the key is present more than once (which it
  // shouldn't be)
  for (i = 0; i < 6; i++) {
    if (0 != k && _keyReport.keys[i] == k) {
      _keyReport.keys[i] = 0x00;
    }
  }

  sendReport(&_keyReport);
  return 1;
}

size_t BeanHid_::_sendKey(uint8_t c) {
  uint8_t p = _holdKey(c);     // Keydown
  uint8_t r = _releaseKey(c);  // Keyup
  return (p & r);
}

int BeanHid_::holdKey(char key) { return (int)_holdKey((uint8_t)key); }
int BeanHid_::holdKey(modifierKey key) { return (int)_holdKey((uint8_t)key); }

/**
 *  Needs docs
 */
int BeanHid_::releaseKey(char key) { return (int)_releaseKey((uint8_t)key); }
int BeanHid_::releaseKey(modifierKey key) {
  return (int)_releaseKey((uint8_t)key);
}

/**
 *  Needs docs
 */
void BeanHid_::releaseAllKeys(void) {
  _keyReport.keys[0] = 0;
  _keyReport.keys[1] = 0;
  _keyReport.keys[2] = 0;
  _keyReport.keys[3] = 0;
  _keyReport.keys[4] = 0;
  _keyReport.keys[5] = 0;
  _keyReport.modifiers = 0;
  sendReport(&_keyReport);
}

/**
 *  Needs docs
 */
int BeanHid_::sendKey(char key) { return (int)_sendKey((uint8_t)key); }
int BeanHid_::sendKey(modifierKey key) { return (int)_sendKey((uint8_t)key); }

/**
 *  Needs docs
 */
int BeanHid_::sendKeys(String charsToType) {
  int status = 0;
  int maxIndex = charsToType.length();
  for (int i = 0; i < maxIndex; i++) {
    status |= _sendKey(charsToType.charAt(i));
    delay(5);  // Characters tend to drop if there's no delay between them
  }

  return status;
}
