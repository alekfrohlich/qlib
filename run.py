#!/usr/bin/python3

import subprocess
import sys

QEMU = 'qemu-system-i386'

if (len(sys.argv) > 1):
    debug = True
else:
    debug = False

if debug:
    subprocess.run([QEMU, '-cdrom', 'bootable_app.iso', '-s', '-S'])
else:
    subprocess.run([QEMU, '-cdrom', 'bootable_app.iso'])
