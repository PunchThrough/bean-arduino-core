Bean - A BLE Arduino By Punch Through Design
=============================================

Repo Overview
-------------

This repo contains the Arduino firmware and installation files used in the LightBlue Bean project. The firmware files are based off of the Arduino firmware release version 1.0.5 and have been modified to work on the LightBlue Bean, a system that has on it a few other features, a BLE radio an accelerometer and a multicolor LED.

### TODO:

* Add note to this doc regarding FUSES (!! BOD)
* Fix Serial.println so, like Serial.print, extra radio messages arn't sent for the newline
* Add proper doxygen comments to relelvent parts, especially the API
* Add features to emulator to provide better, more useful testing
* Add more and better examples

### Reading and Setting Atmega Fuses with avrdude

We're going to need to set some fuses for our part in the factory.  The fuse settings we need are going to set our part to basically should
match the Arduino Pro defaults with brownout disabled.

At the time of writing, I don't have a programmer at my disposal.  However the steps taken should be the 
We write the fuse settings here: XX XX XX

Steps:

1. Install avrdude.  I used [http://www.obdev.at/products/crosspack/index.html](CrossPack) to do this.
1. Read the fuses from an off the shelf ArduinoPro.  Use this command after avrdude is in your path: `avrdude -p atmega328p -c usbtiny -U lfuse:r:-:h -U hfuse:r:-:h -U efuse:r:-:h -U lock:r:-:h`
1. Go to [http://www.engbedded.com/fusecalc/](The Avr Fuse Calculator) and enter in the current state of the fuses
1. Turn off Brown Out Detection (BOD).
1. Calculate the new state of the fuses using the website, note for future (so no one else has to do these silly instructions)
1. Use avrdude to write and test your new fuse settings.  The command line will look something like this (except, these are guessed values: `avrdude -p atmega328p -c usbtiny -U lfuse:w:0xee:m -U hfuse:w:0xdc:m -U efuse:w:0xff:m`)

### Key Files:

#### Everything in the `hardware` directory

The contents of the hardware directory should be copied into the Arduino program resources hardware directory on install.  This directory contains the firmware for the LightBlue Bean, as well as the files needed for the Arduino IDE to recognize the board allow users to program it.

#### Everything in the `examples` directory

The contents of the examples directory should be copied separately on install to the examples resources in the Arduino IDE.

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

### Repo Setup:

This repository uses git submodules and requires a few extra steps for cloning and pulling.

#### Clone

```
$ git clone REPO_URL --recursive
```
#### Initialize Submodules 

(This is unnecessary if the recursive clone works)

```		
$ git submodule update --init --recursive
```

#### Pull
	
```
$ git pull
$ git submodule update --recursive
```

### Code Format

Use `clang-format` to format your code before you check it in... there is already a config file `.clang-format` checked in so just run:

```
$ clang-format -i <file>
```

Or to run it on an entire directory:

```
$ find hardware/bean/ | xargs clang-format -i
```
