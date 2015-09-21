#!/usr/bin/env python
from os import walk
from os.path import join
from sys import exit
import re
import subprocess

linter = 'cpplint'
bean_files = r"Bean.*\.(h|cpp)"

bean_files_regex = re.compile(bean_files)

to_lint = []
for root, dirnames, filenames in walk('.'):
    for filename in filenames:
        if bean_files_regex.match(filename):
            path = join(root, filename)
            to_lint.append(path)

lint_cmd = [linter] + to_lint

return_code = subprocess.call(lint_cmd)
exit(return_code)
