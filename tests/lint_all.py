#!/usr/bin/env python
from os import walk
from os.path import join
from sys import argv, exit
import re
import subprocess

usage = """Usage:
    {script} --show: list all files that are linted
    {script} --reformat: reformat Bean files according to style guide
    {script} --lint: lint Bean files using style guide"""

formatter = ['clang-format', '-i']
linter = ['cpplint']

bean_files = r"Bean.*\.(h|cpp)"

bean_files_regex = re.compile(bean_files)

to_process = []
for root, dirnames, filenames in walk('.'):
    for filename in filenames:
        if bean_files_regex.match(filename):
            path = join(root, filename)
            to_process.append(path)

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
