#!/usr/bin/python3

import subprocess
import sys

QEMU = 'qemu-system-i386' 
BRAE = 'bin/brae.iso'   

if (len(sys.argv) > 1):
    debug = True
else:
    debug = False

if debug:
    subprocess.run([QEMU, '-cdrom', BRAE, '-s', '-S'])
else:
    subprocess.run([QEMU, '-cdrom', BRAE])
