# [LightBlue Bean](https://punchthrough.com/bean) Arduino Library

This repo contains the Arduino firmware and installation files used in the LightBlue Bean project. The firmware files are based off of the Arduino firmware release version 1.0.5 and have been modified to work on the LightBlue Bean, a system that has on it a few other features, a BLE radio an accelerometer and a multicolor LED.

# Reading and Setting Atmega Fuses with avrdude

We're going to need to set some fuses for our part in the factory.  The fuse settings we need are going to set our part to basically should match the Arduino Pro defaults with brownout disabled.

1. Install avrdude.  I used [http://www.obdev.at/products/crosspack/index.html](CrossPack) to do this.
1. Read the fuses from an off the shelf ArduinoPro.  Use this command after avrdude is in your path: `avrdude -p atmega328p -c usbtiny -U lfuse:r:-:h -U hfuse:r:-:h -U efuse:r:-:h -U lock:r:-:h`
1. Go to [http://www.engbedded.com/fusecalc/](The Avr Fuse Calculator) and enter in the current state of the fuses
1. Turn off Brown Out Detection (BOD).
1. Calculate the new state of the fuses using the website, note for future (so no one else has to do these silly instructions)
1. Use avrdude to write and test your new fuse settings.  The command line will look something like this (except, these are guessed values: `avrdude -p atmega328p -c usbtiny -U lfuse:w:0xee:m -U hfuse:w:0xdc:m -U efuse:w:0xff:m`)

# Key Files

## Everything in the `hardware` directory

The contents of the hardware directory should be copied into the Arduino program resources hardware directory on install.  This directory contains the firmware for the LightBlue Bean, as well as the files needed for the Arduino IDE to recognize the board allow users to program it.

## Everything in the `examples` directory

The contents of the examples directory should be copied separately on install to the examples resources in the Arduino IDE.

## Everything in the `beanModuleEmulator` directory

This directory contains an Emulator of the 'non-Arduino' parts of the bean.  This acts as a test jig for the Arduino code, and allows us to verify that things operate as expected.  You can run `python BeanModuleEmulator.py` from within the beanModuleEmulator directory.

# Dependencies

There are a few python library dependencies you'll need to make the emulator work:

* TkInter
* numpy
* pyserial
* enum

The emulator has been tested with python 2.7.6 installed by Homebrew on OSX

# Development

During development, it is recommended to use soft links from inside your Arduino IDE resources to your repository.  It will make it easier to test your work.

# Repo Setup

This repository uses git submodules and requires a few extra steps for cloning and pulling.

## Clone

```
$ git clone REPO_URL --recursive
```

## Initialize Submodules 

(This is unnecessary if the recursive clone works)

```		
$ git submodule update --init --recursive
```

## Pull
	
```
$ git pull
$ git submodule update --recursive
```

# Code Style

Code in this repo should adhere to the [Google C++ Style Guide](https://google-styleguide.googlecode.com/svn/trunk/cppguide.html).

clang-format and cpplint are two tools that can help insure your code follows this standard.

## Installing Tools

```sh
brew install clang-format
pip install cpplint
```

## Auto-Formatting

To reformat one or more files before committing:

```sh
clang-format -i FILE [FILE ...]
```

`-i`: reformat code in-place. Default behavior without this flag is to reformat the file and dump the formatted output to stdout.

clang-format uses the `.clang-format` file to set formatting style. See the [clang-format docs](http://clang.llvm.org/docs/ClangFormat.html) for more info.

## Linting

To lint one or more files for errors before committing:

```sh
cpplint FILE [FILE ...]
```

cpplint is configured in `CPPLINT.cfg`. That file needs to be in your working directory or one of its parents for cpplint to find it. See the [cpplint.py source](https://google-styleguide.googlecode.com/svn/trunk/cpplint/cpplint.py) for more info on configuration options and linter filters.

## Entire Directories

To process an entire directory of files, searching recursively for nested files:

```sh
find <DIR_PATH> | xargs <SOME_COMMAND>
```

Be careful of reformatting or linting library files that you don't actually want to modify.

# TODO

* Fix Serial.println so, like Serial.print, extra radio messages aren't sent for the newline
* Add proper doxygen comments to relelvent parts, especially the API
* Add features to emulator to provide better, more useful testing
* Add more and better examples
