

#ifndef __BEANHID__
#define __BEANHID__

#include "BeanSerialTransport.h"

typedef enum {
  MOUSE_LEFT = 1,
  MOUSE_RIGHT = 2,
  MOUSE_MIDDLE = 4,
  MOUSE_ALL = (MOUSE_LEFT | MOUSE_RIGHT | MOUSE_MIDDLE)
} mouseButtons;

typedef enum {
  KEY_LEFT_CTRL = 0x80,
  KEY_LEFT_SHIFT = 0x81,
  KEY_LEFT_ALT = 0x82,
  KEY_LEFT_GUI = 0x83,
  KEY_RIGHT_CTRL = 0x84,
  KEY_RIGHT_SHIFT = 0x85,
  KEY_RIGHT_ALT = 0x86,
  KEY_RIGHT_GUI = 0x87,

  KEY_UP_ARROW = 0xDA,
  KEY_DOWN_ARROW = 0xD9,
  KEY_LEFT_ARROW = 0xD8,
  KEY_RIGHT_ARROW = 0xD7,
  KEY_BACKSPACE = 0xB2,
  KEY_TAB = 0xB3,
  KEY_RETURN = 0xB0,
  KEY_ESC = 0xB1,
  KEY_INSERT = 0xD1,
  KEY_DELETE = 0xD4,
  KEY_PAGE_UP = 0xD3,
  KEY_PAGE_DOWN = 0xD6,
  KEY_HOME = 0xD2,
  KEY_END = 0xD5,
  KEY_CAPS_LOCK = 0xC1,
  KEY_F1 = 0xC2,
  KEY_F2 = 0xC3,
  KEY_F3 = 0xC4,
  KEY_F4 = 0xC5,
  KEY_F5 = 0xC6,
  KEY_F6 = 0xC7,
  KEY_F7 = 0xC8,
  KEY_F8 = 0xC9,
  KEY_F9 = 0xCA,
  KEY_F10 = 0xCB,
  KEY_F11 = 0xCC,
  KEY_F12 = 0xCD
} modifierKey;

// HID Consumer Usage IDs (subset of the codes available in the USB HID Usage
// Tables spec)
typedef enum {
  POWER = 0x30,
  ASSIGN_SEL = 0x81,
  CHANNEL_UP = 0x9C,
  CHANNEL_DOWN = 0x9D,
  RECALL_LAST = 0x83,
  PLAY = 0xB0,  // Apple's Play/Pause button
  PAUSE = 0xB1,
  RECORD = 0xB2,
  FAST_FORWARD = 0xB3,
  REWIND = 0xB4,
  SCAN_NEXT_TRK = 0xB5,
  SCAN_PREV_TRK = 0xB6,
  STOP = 0xB7,
  VOLUME_UP = 0xE9,
  VOLUME_DOWN = 0xEA,
  MUTE = 0xE2
} mediaControl;

// Low level key report: up to 6 keys and shift, ctrl etc at once
typedef struct {
  uint8_t modifiers;
  uint8_t reserved;
  uint8_t keys[6];
} KeyReport;

typedef struct { uint8_t mouse[4]; } MouseReport;

typedef struct { uint8_t bytes[2]; } CcReport;

class BeanHid_ {
 private:
  void buttons(uint8_t b);
  void _genericSendReport(uint8_t id, uint8_t *buffer, size_t length);
  void sendReport(MouseReport *pReport);
  void sendReport(KeyReport *pReport);
  void sendReport(CcReport *pReport);
  size_t _holdKey(uint8_t c);
  size_t _releaseKey(uint8_t c);
  size_t _sendKey(uint8_t c);

 public:
  BeanHid_(void);
  /****************************************************************************/
  /** @name HID
   * The user must enter a pairing code (default of 000000) to connect.
   * HID allows the bean to act as a mouse, keyboard, or media control device.
   *
   * Important: When disconnecting from a device, you must manually forget Bean in the system's Bluetooth settings as well as Bean Loader.
   */
  ///@{


  /**
   *  This function turns on the Bean's HID functionality.  It will require pairing using a pin code with your device and if you wish to disconnect it you must disconnect through your devices bluetooth system settings menu.
   *
   *  HID may not work well with other profiles such as ANCS or MIDI. Use multiple profiles simultaneously at your own risk.
   *
   *  You may need to power cycle Bean after enabling HID.
   *
   *  # Examples
   *  This sketch demonstrates how to use Bean as a HID peripheral.
   *  Bean will move a mouse pointer around the screen when it's tilted side to side, forward, and back.
   *  Sending ASCII '1' via Virtual Serial will cause Bean to type "Hello, world!", and sending '2' will cause Bean to send the Play media key.
   *
   *  @include profiles/HID.ino
   */
  void enable(void);

  /**
  *  @return True if HID is enabled
  */
  bool isEnabled(void);

  /**
   *  Disables the HID functionality.
   *
   *  If Bean has been paired with another device, you must un-pair Bean from that device manually.
   *
   *  You may need to power cycle Bean after disabling HID.
   */
  void disable(void);

  /**
   *  Holds a key down.  Takes any ascii character.  Can hold a max of 6 character keys.
   *  @param key single character key
   *  @return 1 if success 0 if failure
   */
  int holdKey(char key);

  /**
   *  Holds a key down. takes any modifier key specified by modifierKey.
   *  Does not affect a users keyboard (sending hold shift won't type capital letters when you press them on your keyboard)
   *  @param key takes a modifier key to hold down.  Can hold down all 8 of the first 8 modifier keys.  The keys that come after the first 8 are counted as standard keys and therefore part of the 6 character keys.
   *
   */
  int holdKey(modifierKey key);

  /**
   *  Releases a key.
   *  @param key the key to release in character form
   */
  int releaseKey(char key);

  /**
   *  Releases a key.
   *  @param key the key to release of type modifierKey
   *  @return 1 if success 0 if failure
   */
  int releaseKey(modifierKey key);

  /**
   *  Releases all currently held keys
   */
  void releaseAllKeys();

  /**
   *  Sends a full keypress event.  Like calling holdKey then releaseKey
   *  @param key the key to send in character form
   *  @return 1 if success 0 if failure
   */
  int sendKey(char key);

  /**
   *  Sends a full keypress event.  Like calling holdKey then releaseKey
   *  @param key the key to send of type modifierKey
   *  @return 1 if success 0 if failure
   */
  int sendKey(modifierKey key);

  /**
   *  Sends a string of characters as keyboard events
   *  @param charsToType a String of characters for the keyboard to emulate
   *  @return 1 if success 0 if failure
   */
  int sendKeys(String charsToType);

  /**
   *  Sends a mouse move command
   *  @param delta_x a signed 8 bit value for how many pixels to move the mouse in the x direction
   *  @param delta_y a signed 8 bit value for how many pixels to move the mouse in the y direction
   *  @param delta_wheel an optional signed 8 bit balue for how many clicks to move the mouse wheel
   */
  void moveMouse(signed char delta_x, signed char delta_y,
                 signed char delta_wheel = 0);

  /**
   *  Holds a mouse button down
   *  @param button the button to hold of type mouseButtons.  Defaults to MOUSE_LEFT.
   */
  void holdMouseClick(mouseButtons button = MOUSE_LEFT);

  /**
   *  Releases a mouse button.
   *  @param button the button to release of type mouseButtons.  Defaults to MOUSE_LEFT.
   */
  void releaseMouseClick(mouseButtons button = MOUSE_LEFT);

  /**
   *  Sends a mouse hold then a mouse release command.
   *  @param button the button to click of type mouseButtons.  Defaults to MOUSE_LEFT.
   */
  void sendMouseClick(mouseButtons button = MOUSE_LEFT);

  /**
   *  Sends a media control command command of type mediaControl.
   *  @param command takes a media control command of type mediaControl
   */
  void sendMediaControl(mediaControl command);

  /**
   *  Holds a media control command command of type mediaControl.
   *  @param command takes a media control command of type mediaControl
   */
  void holdMediaControl(mediaControl command);

  /**
   *  Releases a media control command command of type mediaControl.
   *  @param command takes a media control command of type mediaControl
   */
  void releaseMediaControl(mediaControl command);

  /**
   *  Releases all currently held media control commands
   */
  void releaseAllMediaControls();
  ///@}

 private:
  bool isShiftHeld;
};
extern BeanHid_ BeanHid;

#endif /* if defined(BeanHID) */
