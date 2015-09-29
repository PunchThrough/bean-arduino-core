#!/bin/sh

# Installing PlatformIO with pip is kind of broken. Just use their install script.
python -c "$(curl -fsSL https://raw.githubusercontent.com/platformio/platformio/master/scripts/get-platformio.py)"
platformio platforms install atmelavr
mkdir -p ~/.platformio/boards  # -p: no error if cached /boards already exists
cp tests/resources/platformio/boards/punchthrough.json ~/.platformio/boards/
cp -rvf hardware/bean/avr/* ~/.platformio/packages/framework-arduinoavr/
