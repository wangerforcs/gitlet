import sys, re
from subprocess import \
     check_output, PIPE, STDOUT, DEVNULL, CalledProcessError, TimeoutExpired
from os.path import abspath, basename, dirname, exists, join, splitext
from getopt import getopt, GetoptError
from os import chdir, environ, getcwd, mkdir, remove, access, W_OK
from shutil import copyfile, rmtree
from math import log

out = check_output(["./gitlet", "init"], stderr=STDOUT)
print(out.decode("utf-8"))