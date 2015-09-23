#!/usr/bin/env python
"""Run all combinations of boards and test sketches."""
from glob import glob
from sys import exit
import subprocess

compiler_configs = [
    # Adding more boards? Add them to this array:
    # ['platformio', 'ci', '--board=your-board-here'],
    ['platformio', 'ci', '--board=lightblue-bean'],
]

test_sketches = glob('tests/resources/test_sketches/*.ino')

return_code = 0
for sketch in test_sketches:
    for compiler in compiler_configs:
        env = {'PLATFORMIO_CI_SRC': sketch}
        try:
            subprocess.check_call(compiler, env=env)
        except subprocess.CalledProcessError:
            return_code = 1

exit(return_code)
