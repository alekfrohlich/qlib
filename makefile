.PHONY: all brae image clean

# architecture
export TARGET = i686-elf

# C++ compiler, linker
export CXX = $(TARGET)-g++
export AS  = $(TARGET)-as

# directories
export SRCDIR = $(CURDIR)/src
export INCDIR = $(CURDIR)/include
export IMGDIR = $(CURDIR)/img
export DIRS   = src include img

# toolchain configuration
export CFLAGS  = -g -I $(INCDIR)
export CFLAGS += -O2 -Wall -Wextra
export CFLAGS += -ffreestanding -fno-exceptions
export CFLAGS += -fno-rtti -nostdlib

all: brae image

brae:
	cd $(SRCDIR) && $(MAKE) all

image:
	cd $(IMGDIR) && $(MAKE) all

clean:
	cd $(SRCDIR) && $(MAKE) clean
	cd $(IMGDIR) && $(MAKE) clean	
