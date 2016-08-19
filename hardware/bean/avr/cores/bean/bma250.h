#ifndef BEAN_BMA250_H
#define BEAN_BMA250_H

#define REG_X_ACCEL_LSB         0x02
#define REG_Y_ACCEL_LSB         0x04
#define REG_Z_ACCEL_LSB         0x06

#define REG_G_SETTING           0x0F
#define VALUE_2G                0x03
#define VALUE_4G                0x05
#define VALUE_8G                0x08
#define VALUE_16G               0x0C

// Normal mode:     0x00    (~140uA)
// Suspend mode:    0x80    (~0.5uA)
// Low Power 10ms:  0x54    (~16.4uA)
// Low Power 100ms: 0x5A    (~2.3uA)
// Low Power 1s:    0x5E    (~0.7uA)
#define REG_POWER_MODE_X11      0x11
#define VALUE_NORMAL_MODE       0x00
#define VALUE_SUSPEND_MODE      0x80
#define VALUE_LOW_POWER_10MS    0x54
#define VALUE_LOW_POWER_100MS   0x5A
#define VALUE_LOW_POWER_1S      0x5E

#define REG_INT_STATUS_X09      0x09
#define CHECK_FLAT_INT          0x80
#define CHECK_ORIENT_INT        0x40
#define CHECK_SINGLE_TAP_INT    0x20
#define CHECK_DOUBLE_TAP_INT    0x10
#define CHECK_ANY_MOTION_INT    0x04
#define CHECK_HIGH_G_INT        0x02
#define CHECK_LOW_G_INT         0x01

#define REG_INT_SETTING_X16     0x16
#define ENABLE_FLAT_INT         0x8000
#define ENABLE_ORIENT_INT       0x4000
#define ENABLE_SINGLE_TAP_INT   0x2000
#define ENABLE_DOUBLE_TAP_INT   0x1000
#define ENABLE_SLOPE_Z_INT      0x0400
#define ENABLE_SLOPE_Y_INT      0x0200
#define ENABLE_SLOPE_X_INT      0x0100
#define ENABLE_ANY_MOTION_INT   0x0700

#define REG_INT_SETTING_X17     0x17
#define ENABLE_NEW_DATA_INT     0x0010
#define ENABLE_LOW_G_INT        0x0008
#define ENABLE_HIGH_G_Z_INT     0x0004
#define ENABLE_HIGH_G_Y_INT     0x0002
#define ENABLE_HIGH_G_X_INT     0x0001

#define REG_INT_MAPPING_X19     0x19
#define MASK_X19_ALL_INT1       0xF7
#define WAKE_FLAT_INT  	    	  0x80
#define WAKE_ORIENT_INT      	  0x40
#define WAKE_SINGLE_TAP_INT     0x20
#define WAKE_DOUBLE_TAP_INT     0x10
#define WAKE_ANY_MOTION_INT     0x04
#define WAKE_HIGH_G_INT         0x02
#define WAKE_LOW_G_INT          0x01


#define REG_INT_MAPPING_X1A     0x1A
#define MASK_X1A_ALL_INT1       0x81

#define REG_LATCH_CFG_X21       0x21
#define VALUE_LATCHED           0x07
#define VALUE_TEMPORARY_250MS   0x01
#define MASK_RESET_INT_LATCH    0x80

#endif //BEAN_BMA250_H
