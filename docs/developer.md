[<< Back](../README.md)

# Developer Guide

This document describes the following:

* [Project Layout](#project-layout)
* [Contributing](#contributing)
* [Release Process](#release-process)

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

# Contributing

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

Code in this repo should adhere to the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html).

This repo comes with tools to ensure your code meets style guidelines. If code you submit for review does not fit our style guide, your pull request will fail our automated tests. Lint your code before submitting it to avoid this.

## Installing Tools

If you're running the reformatter or linter for the first time, run this first to install the tools:

```sh
make install
```

Are you getting the following error?

> Agreeing to the Xcode/iOS license requires admin privileges, please re-run as root via sudo.

This is an Xcode problem. Run `sudo xcodebuild` to agree with the Xcode license agreement, then run `make install` again.

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

## Auto-Reformatting

clang-format is set up to use the Google style guide. Reformatting your code will clean up many of the lint errors you might make.

To reformat Bean files in this project:

```sh
make reformat
```

# Release Process
