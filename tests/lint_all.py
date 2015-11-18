#!/usr/bin/env python
"""List, reformat, or lint Bean project files."""
from os import walk
from os.path import join, relpath
from sys import argv, exit
from glob import glob
import re
import subprocess

usage = """Usage:
    {script} --show: list all files that are linted
    {script} --reformat: reformat modified Bean files according to style guide
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

# git status --porcelain provides machine-readable git statuses
# flags below generated from https://git-scm.com/docs/git-status short format
valid_git_statuses = [' M', ' D', 'M ', 'MM', 'MD', 'A ', 'AM', 'AD', 'D ',
                      'DM', 'R ', 'RM', 'RD', 'C ', 'CM', 'CD', 'M ', 'A ',
                      'R ', 'C ', ' M', 'MM', 'AM', 'RM', 'CM', ' D', 'MD',
                      'AD', 'RD', 'CD']

valid_extensions_arg = '--extensions={}'.format(','.join(valid_extensions))
linter.append(valid_extensions_arg)

file_regexes_compiled = [re.compile(rx) for rx in file_regexes]

# Linted files: all files matched by file_regexes and file_globs
to_lint = []
for root, dirnames, filenames in walk('.'):
    for filename in filenames:
        for file_regex in file_regexes_compiled:
            if file_regex.match(filename):
                path = join(root, filename)
                to_lint.append(path)
for g in file_globs:
    to_lint.extend(glob(g))

# Reformatted files: all files that are modified in git AND are already in the
# list of files to be linted
to_reformat = []
git_status_raw = (subprocess.check_output(['git', 'status', '--porcelain'])
                  .decode().strip('\n'))
git_status_lines = git_status_raw.split('\n')
dirty_files = []
for line in git_status_lines:
    status = line[:2]
    path = line[3:]
    if status in valid_git_statuses:
        to_reformat.append(path)

# Normalize file paths
to_lint = [relpath(p) for p in to_lint]
to_reformat = [relpath(p) for p in to_reformat]

# Don't reformat files that we aren't linting
to_reformat = [p for p in to_reformat if p in to_lint]

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

if show:
    if to_lint:
        print('Files to be linted:')
        for fn in to_lint:
            print('   ', fn)
    else:
        print('No files to be linted')
    if to_reformat:
        print('Files to be reformatted:')
        for fn in to_reformat:
            print('   ', fn)
    else:
        print('No files to be reformatted')

if reformat:
    format_cmd = formatter + to_reformat
    reformat_code = subprocess.call(format_cmd)
    if lint:
        if reformat_code:  # We only get one exit code, use it for the linter
            print('Reformatter exited with error code', reformat_code)
    else:
        exit(reformat_code)

if lint:
    lint_cmd = linter + to_lint
    lint_code = subprocess.call(lint_cmd)
    exit(lint_code)
