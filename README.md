Bean - A BLE Arduino By Punch Through Design
=============================================

Repo Overview
-------------

This repo contains the Arduino firmware and installation files used in the LightBlue Bean project. The firmware files are based off of the Arduino firmware release version 1.0.5 and have been modified to work on the LightBlue Bean, a system that has on it a few other features, a BLE radio an accelerometer and a multicolor LED.

### Key Files:

#### Everything in the `hardware` directory
The contents of the hardware directory should be copied into the Arduino program resources hardware directory on install.  This directory contains the firmware for the LightBlue Bean, as well as the files needed for the Arduino IDE to recognize the board allow users to program it.

#### Everything in the `examples` directory
The contents of the examples directory should be copied separately on install to the exaples resources in the Arduino IDE.

#### Everything in the `beanModuleEmulator` directory
This directory contains an Emulator of the 'non-Arduino' parts of the bean.  This acts as a test jig for the Arduino code, and allows us to verify that things operate as expected.  You can run `python BeanModuleEmulator.py` from within the beanModuleEmulator directory.

##### Dependencies

There are a few python library dependencies you'll need to make the emulator work:

* TkInter
* numpy
* pyserial
* enum

The emulator has been tested with python 2.7.6 installed by Homebrew on OSX

### Development:
During development, it is recommended to use soft links from inside your Arduino IDE resources to your repository.  It will make it easier to test your work.