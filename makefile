SHELL := /bin/bash

export MAKEINC = $(CURDIR)/makeinc
include $(MAKEINC)

.PHONY: clean distclean format tools

#=============================================================================#
# @TODO: refactor inline bash in order to avoid testing for .debug.lock
#=============================================================================#

#_______SOURCE CODE___________________________________________________________#

.SILENT: all _all debug _debug
.PHONY: all _all debug _debug

MAKEFLAGS += --no-print-directory
CXXFLAGS += $(_ALL_CXXFLAGS)
CXXFLAGS += $(_DEBUG_CXXFLAGS)
_DEBUG_EXT :=
FATBIN = $(addsuffix $(_DEBUG_EXT),$(GRUB)/brae.bin)

all:
	$(MAKE) _all

_all:
	$(MAKE) _ALL_CXXFLAGS=-O2 $(FATBIN)
	$(MAKE) $(ISOFILE)

debug:
	if ! test -f ".debug.lock"; then \
		$(MAKE) clean && touch .debug.lock; \
	fi
	$(MAKE) _debug

_debug:
	$(MAKE) _DEBUG_CXXFLAGS="-Og -Wall -Wextra" _DEBUG_EXT=.debug $(FATBIN).debug
	$(MAKE) $(ISOFILE)

C_SRC 	:= $(wildcard TRGT_ARCH/*.c)

CXX_SRC :=  $(wildcard $(TRGT_MACH)/*.cc) \
			$(wildcard $(STD)/*.cc) \
			$(wildcard $(APP)/*.cc)

OBJS  	:= $(CXX_SRC:.cc=.o)

# the final executable must be linked in this exact order
OBJ_LINK_LIST := $(TRGT_ARCH)/crt0.o $(TRGT_ARCH)/crtend.o $(OBJS) $(TRGT_ARCH)/lib_init.o $(TRGT_ARCH)/crtbegin.o

# ld expects to find crtend.o and crtbegin.o relative to PWD,
# it fails if given /path/to/crtbegin.o
$(FATBIN): $(OBJ_LINK_LIST)
	cd $(TRGT_ARCH) && $(LD) $(LDFLAGS) crt0.o crtend.o $(OBJS) lib_init.o crtbegin.o -lgcc -o $@

CPPFLAGS := -MD -MP

%.o: %.S
	 $(AS) $(ASFLAGS) $< -o $@

-include $(CXX_SRC:.cc=.d)
-include $(C_SRC:.c=.d)

#_______BOOTABLE GRUB IMAGE___________________________________________________#

$(ISOFILE):
	grub-mkrescue -o $@ $(IMG)

#_______CLANG-FORMAT__________________________________________________________#

format:
	cd $(DOCS) && find .. -regex '.*\.\(cc\|c\|h\)'\
		 -exec clang-format-8 style=.clang-format -i {} \;

#_______CROSS-CHAIN SETUP_____________________________________________________#

#=============================================================================#
# @TODO: add .PHONY bash script to automate tool installation
#=============================================================================#

tools:
	@echo $(shell $(CXX) $(CXXFLAGS) -MM -MG src/std/ostream.cc)

clean:
	@find . -type f \( -name "*.o" -o -name "*.d" \) -delete
	@rm -f .debug.lock brae.iso

distclean: clean
	@rm -f img/boot/brae.bin img/boot/brae.bin.debug