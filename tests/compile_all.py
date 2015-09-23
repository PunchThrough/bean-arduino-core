#!/usr/bin/env python
"""Run all combinations of boards and test sketches."""
from glob import glob
from sys import exit
from os import environ
import subprocess

compiler_configs = [
    # Adding more boards? Add them to this array:
    # ['platformio', 'ci', '--board=your-board-here'],
    ['platformio', 'ci', '--board=lightblue-bean'],
]

test_sketches = [
    'tests/resources/test_sketches/*.ino',
    'examples/LightBlueBean/01.Basics/*/*.ino',
]

test_sketch_paths = []
for pattern in test_sketches:
    test_sketch_paths.extend(glob(pattern))

return_code = 0
for sketch_path in test_sketch_paths:
    for compiler in compiler_configs:
        # Add this variable on top of the existing env to preserve
        # the virtualenv which contains the PlatformIO executable
        env = environ.copy()
        env['PLATFORMIO_CI_SRC'] = sketch_path
        try:
            subprocess.check_call(compiler, env=env)
        except subprocess.CalledProcessError:
            return_code = 1

exit(return_code)
