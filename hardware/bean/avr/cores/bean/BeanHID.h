

#ifndef __BEANHID__
#define __BEANHID__


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
  PLAY = 0xB0,
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


class BeanHid_ {

  /****************************************************************************/
  /** @name HID
   *  Use your Bean as a Human Interface Device to emulate a keyboard or mouse.
   */
  ///@{

  /**
   *  Needs docs
   */
  void enable(void);

   /**
   *  Needs docs
   */
  bool isEnabled(void);

  /**
   *  Needs docs
   */
  bool disable(void);

  /**
   *  Needs docs
   */
  int holdKey(char key);
  int holdKey(modifierKey key);

  /**
   *  Needs docs
   */
  int releaseKey(char key);
  int releaseKey(modifierKey key);

  /**
   *  Needs docs
   */
  int releaseAllKeys();

  /**
   *  Needs docs
   */
  int sendKey(char key);
  int sendKey(modifierKey key);

  /**
   *  Needs docs
   */
  int sendKeys(String charsToType);

  /**
   *  Needs docs
   */
  void moveMouse(signed char delta_x, signed char delta_y, signed char delta_wheel = 0);

  /**
   *  Needs docs
   */
  void holdMouseClick(mouseButtons button = MOUSE_LEFT);

  /**
   *  Needs docs
   */
  void releaseMouseClick(mouseButtons button = MOUSE_LEFT);

  /**
   *  Needs docs
   */
  void sendMouseClick(mouseButtons button = MOUSE_LEFT);

  /**
   *  Needs docs
   */
  void sendMediaControl(mediaControl command);

  /**
   * Needs docs
   */
  void holdMediaControl(mediaControl command);

  /**
   * Needs docs
   */
  void releaseMediaControl(mediaControl command);

  /**
   * Needs docs
   */
  void releaseAllMediaControls();
  ///@}
};
extern BeanHid_ BeanHid;
//==============================================================================
//==============================================================================
// Mouse



class BeanMouse_ {

 public:
  BeanMouse_(void);

  void click(uint8_t b = MOUSE_LEFT);
  void release(uint8_t b = MOUSE_LEFT);
  void move(signed char x, signed char y, signed char wheel = 0);
  void press(uint8_t b = MOUSE_LEFT);      // press LEFT by default

};
extern BeanMouse_ BeanMouse;

//==============================================================================
//==============================================================================
// Keyboard



class BeanKeyboard_ : public Print {

 public:
  BeanKeyboard_(void);

  void sendCC(uint8_t command);
  void holdCC(uint8_t command);
  void releaseCC(uint8_t command);

  virtual size_t write(uint8_t k);
  virtual size_t press(uint8_t k);
  virtual size_t release(uint8_t k);
  virtual void releaseAll(void);
};
extern BeanKeyboard_ BeanKeyboard;


#endif /* if defined(BeanHID) */
