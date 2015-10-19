#!/usr/bin/env python
"""List, reformat, or lint Bean project files."""
from os import walk
from os.path import join
from sys import argv, exit
from glob import glob
import re
import subprocess

usage = """Usage:
    {script} --show: list all files that are linted
    {script} --reformat: reformat Bean files according to style guide
    {script} --lint: lint Bean files using style guide"""

formatter = ['clang-format', '-i']
linter = ['cpplint']

file_regexes = [
    r"Bean.*\.(h|cpp)"
]
file_globs = [
    'examples/**/*.ino'
]
valid_extensions = [
    'h',
    'cpp',
    'ino'
]

valid_extensions_arg = '--extensions={}'.format(','.join(valid_extensions))
linter.append(valid_extensions_arg)

file_regexes_compiled = [re.compile(rx) for rx in file_regexes]

to_process = []
for root, dirnames, filenames in walk('.'):
    for filename in filenames:
        for file_regex in file_regexes_compiled:
            if file_regex.match(filename):
                path = join(root, filename)
                to_process.append(path)

for g in file_globs:
    to_process.extend(glob(g))

show = False
reformat = False
lint = False
for arg in argv[1:]:
    if arg == '--show':
        show = True
    if arg == '--reformat':
        reformat = True
    if arg == '--lint':
        lint = True

if not (show or lint or reformat):
    print(usage.format(script=argv[0]))
    exit(1)

format_cmd = formatter + to_process
lint_cmd = linter + to_process

if show:
    print('Files to be linted:')
    for fn in to_process:
        print('   ', fn)

if reformat:
    reformat_code = subprocess.call(format_cmd)
    if lint:
        if reformat_code:  # We only get one exit code, use it for the linter
            print('Reformatter exited with error code', reformat_code)
    else:
        exit(reformat_code)

if lint:
    lint_code = subprocess.call(lint_cmd)
    exit(lint_code)
