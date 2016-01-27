# LightBlue Bean Arduino Core

This repo contains the Arduino firmware and installation files used in the [LightBlue Bean](https://punchthrough.com/bean) project. The firmware files are based off of the Arduino firmware release version 1.0.5 and have been modified to work on the LightBlue Bean. Bean has components that include a BLE radio, an accelerometer, and a multicolor LED.

[Reference Docs](https://punchthrough.com/bean/reference)

# Project Layout

## /hardware

This directory contains the firmware for the LightBlue Bean, as well as the files needed for the Arduino IDE to recognize the board and allow users to program it. Its contents are copied to Arduino IDE's `hardware` directory when Bean Loader is installed on OS X or Windows.

## /examples

This directory contains example Arduino sketches for use with Bean. Its contents are copied to Arduino IDE's `examples` directory when Bean Loader is installed.

## /beanModuleEmulator

This directory contains an emulator for the "non-Arduino" parts of Bean. This acts as a test jig for the Arduino code, and allows us to verify that things operate as expected.

You can run `python BeanModuleEmulator.py` from within the beanModuleEmulator directory. There are a few Python library dependencies you'll need to make the emulator work:

* TkInter
* numpy
* pyserial
* enum

The emulator has been tested with Python 2.7.6 installed via Homebrew on OS X.

# Development

## Testing in Arduino IDE

You can use a symlink to install the development copy of Bean Arduino Core into your Arduino IDE's Hardware folder.

Here's a working command for OS X users:

```sh
ln -s "/absolute/path/to/bean-arduino-core-private/hardware/bean/" ~/Documents/Arduino/hardware
```

Make sure to use an *absolute path*, not a relative path, as the source path for the symlink.

## Pull Requests

All pull requests should be against the `develop` branch.

## Repo Setup

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

# Code Style and Quality

Code in this repo should adhere to the [Punch Through C++ Style Guide](http://punchthrough.github.io/styleguide/cpp.html).

This repo comes with tools to ensure your code meets style guidelines. If code you submit for review does not fit our style guide, your pull request will fail our automated tests. Lint your code before submitting it to avoid this.

## Installing Tools

If you're running the reformatter or linter for the first time, run this first to install the tools:

```sh
make install
```

Are you getting the following error?

> Agreeing to the Xcode/iOS license requires admin privileges, please re-run as root via sudo.

This is an Xcode problem. Run `sudo xcodebuild` to agree with the Xcode license agreement, then run `make install` again.

## Auto-Reformat

clang-format is set up to use the Google style guide. Reformatting your code will clean up many of the lint errors you might make.

To reformat Bean files in this project:

```sh
make reformat
```

clang-format uses the `.clang-format` file to set formatting style. See the [clang-format docs](http://clang.llvm.org/docs/ClangFormat.html) for more info.

## Linting

Our continuous integration server lints all code when it's submitted to a pull request. You can run the linter on your machine to make sure your code is up to this repo's standards.

To lint Bean files for errors before committing:

```sh
make lint
```

cpplint is configured in `CPPLINT.cfg`. See the [cpplint.py source](https://google-styleguide.googlecode.com/svn/trunk/cpplint/cpplint.py) for more info on configuration options and linter filters.

## View List of Linted Files

To see which files will be linted or reformatted:

```sh
make show
```

# Reading and Setting ATmega Fuses with avrdude

We're going to need to set some fuses for our part in the factory. The fuse settings should match the Arduino Pro defaults, with the exception of disabling brownout protection.

1. Install avrdude. I used [http://www.obdev.at/products/crosspack/index.html](CrossPack) to do this.
1. Read the fuses from an off-the-shelf Arduino Pro. Use this command after avrdude is in your path: `avrdude -p atmega328p -c usbtiny -U lfuse:r:-:h -U hfuse:r:-:h -U efuse:r:-:h -U lock:r:-:h`
1. Go to [http://www.engbedded.com/fusecalc/](The Avr Fuse Calculator) and enter in the current state of the fuses
1. Turn off Brown Out Detection (BOD).
1. Calculate the new state of the fuses using the website, note for future (so no one else has to do these silly instructions)
1. Use avrdude to write and test your new fuse settings. The command line will look something like this (except, these are guessed values): `avrdude -p atmega328p -c usbtiny -U lfuse:w:0xee:m -U hfuse:w:0xdc:m -U efuse:w:0xff:m`

# TODO

* Fix Serial.println so, like Serial.print, extra radio messages aren't sent for the newline
* Add features to emulator to provide better, more useful testing
