SHELL := /bin/bash

export MAKEINC = $(CURDIR)/makeinc
include $(MAKEINC)

.PHONY: clean distclean format tools

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%#
# @TODO: refactor inline bash in order to avoid testing for .debug.lock
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%#

#_______SOURCE CODE___________________________________________________________#

.SILENT: all _all debug _debug
.PHONY: all _all debug _debug

all:
	if test -f ".debug.lock"; then \
		$(MAKE) clean; \
	fi
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
OBJ_LINK_LIST := $(addprefix $(TRGT_ARCH)/, crt0.o crtend.o) \
				 $(OBJS) $(addprefix $(TRGT_ARCH)/, lib_init.o crtbegin.o)

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%#
# @TODO: fix ld include error: ld expects to find crtend.o and crtbegin.o
# to be relative to the working directory. It fails when given a relative path.
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%#

$(FATBIN): $(OBJ_LINK_LIST)
	cd $(TRGT_ARCH) && $(LD) $(LDFLAGS) crt0.o crtend.o $(OBJS) \
		lib_init.o crtbegin.o -lgcc -o $@

# currently used to assamble crt0.S
%.o: %.S
	 $(AS) $(ASFLAGS) $< -o $@

#=============AUTOMAKE========================================================#
# Since it's possible to include files within GNU make, this build system
# relies on GCC preprocessor's flags (CPPFLAGS) -MD -MP to generate make
# compliant target rules for each .c and .cc file. It does so by including
# the generated .d files. GNU make is able to turn .cc and .c files into
# dependency files, .d implicitly by using the aforementioned C-preprocessor.
#
# For more information on this technique please refer to the links:
#  - http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/
#  - http://www.microhowto.info/howto/automatically_generate_makefile_
# 	dependencies.html
#=============================================================================#

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

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%#
# @TODO: add .PHONY bash script to automate tool installation
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%#

tools:

#_______CLEAN ENVIRONMENT______________________________________________________#

clean:
	@find . -type f \( -name "*.o" \) -delete
	@rm -f .debug.lock brae.iso

distclean: clean
	@find . -type f -name "*.d" -delete
	@rm -f img/boot/brae.bin img/boot/brae.bin.debug