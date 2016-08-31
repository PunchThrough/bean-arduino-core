#!/usr/bin/env python


import os
import shutil
import subprocess

# yolo
GIT_VERSION = subprocess.check_output('git describe', shell=True).split('-')[0]
BUNDLE_NAME = 'bean-arduino-core-{}'.format(GIT_VERSION)

THIS_DIR = os.path.abspath(os.path.dirname(__file__))
TMP_DIR = os.path.join(THIS_DIR, '..', 'tmp')
EXAMPLES_DIR = os.path.join(THIS_DIR, '..', 'examples')
BUNDLE_PATH = os.path.join(TMP_DIR, BUNDLE_NAME)
BUNDLE_HARDWARE_PATH = os.path.join(BUNDLE_PATH, 'hardware', 'LightBlue-Bean')
BUNDLE_EXAMPLES_PATH = os.path.join(BUNDLE_PATH, 'examples', 'LightBlue-Bean')
CORE_PATH = os.path.join(THIS_DIR, '..', 'hardware', 'bean')


def clean():
    shutil.rmtree(BUNDLE_PATH, ignore_errors=True)
    os.makedirs(BUNDLE_HARDWARE_PATH)
    os.makedirs(BUNDLE_EXAMPLES_PATH)


def copy_core():
    shutil.copytree(CORE_PATH, os.path.join(BUNDLE_HARDWARE_PATH, 'bean'))


def copy_examples():
    for dirpath, dirnames, filenames in os.walk(EXAMPLES_DIR):
        for f in filenames:
            category = dirpath.split('/')[-1].capitalize()
            category_path = os.path.join(BUNDLE_EXAMPLES_PATH, category)
            if not os.path.exists(category_path):
                os.makedirs(category_path)
            filename, extension = os.path.splitext(f)
            if extension == '.ino':
                orig_sketch_path = os.path.join(dirpath, f)
                bundle_sketch_dir = os.path.join(category_path, filename)
                bundle_sketch_path = os.path.join(bundle_sketch_dir, f)
                os.makedirs(bundle_sketch_dir)
                shutil.copy(orig_sketch_path, bundle_sketch_path)


def bundle():
    os.chdir(TMP_DIR)
    zipcmd = "tar -zcvf {}.tar.gz {}".format(BUNDLE_NAME, BUNDLE_NAME)
    subprocess.check_output(zipcmd, shell=True)


clean()
copy_core()
copy_examples()
bundle()
print('Bundle success')
