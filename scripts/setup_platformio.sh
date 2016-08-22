#!/bin/sh

# Installing PlatformIO with pip is kind of broken. Just use their install script.
python -c "$(curl -fsSL https://raw.githubusercontent.com/platformio/platformio/master/scripts/get-platformio.py)"
platformio platforms install atmelavr

# Add Punch Through boards definition
# -p: no error if cached /boards already exists
mkdir -p ~/.platformio/boards
cp resources/platformio/boards/* ~/.platformio/boards/

# Compile once to initialize framework-arduinoavr and ignore errors
platformio ci --board=uno || true

# Add Punch Through core files
# packages/framework-arduinoavr may not exist until the first compile happens,
# so create the directory right away
mkdir -p /home/ubuntu/.platformio/packages/framework-arduinoavr/
cp -rvf hardware/bean/avr/* ~/.platformio/packages/framework-arduinoavr/
