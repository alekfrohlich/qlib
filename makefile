.PHONY: all brae image clean

export TARGET = i686-elf

# C++ compiler, linker
export CXX = $(TARGET)-g++
export AS  = $(TARGET)-as

# directories
export SRCDIR = $(CURDIR)/src
export INCDIR = $(CURDIR)/include

# toolchain configuration
export CFLAGS  = -g -I $(INCDIR)
export CFLAGS += -O2 -Wall -Wextra
export CFLAGS += -ffreestanding -fno-exceptions -fno-rtti -nostdlib

.PHONY: all brae image clean

all: brae image

brae:
	cd $(SRCDIR) && $(MAKE) brae

image:
	cd $(SRCDIR) && $(MAKE) image

clean:
	cd $(SRCDIR) && $(MAKE) clean