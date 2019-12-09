SHELL := /bin/bash

export MAKEINC = $(CURDIR)/makeinc
include $(MAKEINC)

.PHONY: all clean distclean format image tools

#=============================================================================#
# @TODO: refactor inline bash to avoid entering/leaving message from make
#=============================================================================#

#_______SOURCE CODE___________________________________________________________#

.SILENT: all debug

all:
	if test -f ".debug.lock"; then \
    	$(MAKE) -s clean; \
	fi
	$(MAKE) _all

_all: CXXFLAGS += -O2
_all: $(FATBIN) $(ISOFILE)

debug:
	if ! test -f ".debug.lock"; then \
    	$(MAKE) -s clean && touch .debug.lock; \
	fi
	@$(MAKE) _debug

_debug: CXXFLAGS += -Og -Wall -Wextra
_debug: FATBIN = $(GRUB)/brae.bin.debug
_debug: $(FATBIN) $(ISOFILE)

CXX_SRC :=  $(wildcard $(TRGT_MACH)/*.cc) \
			$(wildcard $(STD)/*.cc) \
		    $(wildcard $(APP)/*.cc)

OBJS  	:= $(CXX_SRC:.cc=.o)

# the final executable must be linked in this exact order
OBJ_LINK_LIST := $(TRGT_ARCH)/crt0.o $(TRGT_ARCH)/crtend.o $(OBJS) $(TRGT_ARCH)/lib_init.o $(TRGT_ARCH)/crtbegin.o

# ld expects to find crtend.o and crtbegin.o relative to PWD,
# it fails if given /path/to/crtbegin.o
$(FATBIN): $(OBJ_LINK_LIST)
	cd $(TRGT_ARCH) && $(LD) $(LDFLAGS) crt0.o crtend.o $(OBJS) lib_init.o crtbegin.o -lgcc -o $(FATBIN)

%.o: %.S
	 $(AS) $(ASFLAGS) $< -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

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

clean:
	@find . -type f \( -name "*.o" -o -name "brae.iso" \) -delete
	@rm -f .debug.lock

distclean: clean
	@rm -f img/boot/brae.bin img/boot/brae.bin.debug