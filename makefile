SHELL := /bin/bash

#_______INCLUDES______________________________________________________________#

export MAKEINC = $(CURDIR)/makeinc
include $(MAKEINC)

#_______SOURCE CODE___________________________________________________________#

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%#
# @TODO: Refactor inline bash in order to avoid testing for .debug.lock .
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%#

.SILENT: all _all debug _debug
.PHONY: all _all debug _debug

# see makeinc's CONCURRENT BUILD section for more explanation on .debug.lock

all:
	if test -f ".debug.lock"; then \
		$(MAKE) clean; \
	fi
	$(MAKE) _all

_all:
	$(MAKE) _ALL_CXXFLAGS=-O2 $(FATBIN)

	# delete debug symbols from libgcc.a
	# strip -d $(FATBIN)
	$(MAKE) $(ISOFILE)

debug:
	if ! test -f ".debug.lock"; then \
		$(MAKE) clean && touch .debug.lock; \
	fi
	$(MAKE) _debug

_debug:
	$(MAKE) _DEBUG_CXXFLAGS="-g" _DEBUG_EXT=.debug \
		$(FATBIN).debug
	$(MAKE) $(ISOFILE)

C_SRC 	:= $(wildcard TRGT_ARCH/*.c)

# @TODO: make app-target a cli argument

CXX_SRC :=  $(wildcard $(TRGT_MACH)/*.cc) \
	$(wildcard $(QLIB)/*.cc) \
	$(wildcard $(APP)/hello.cc)

OBJS  	:= $(CXX_SRC:.cc=.o)

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%#
# @TODO: Move cpu.o before OBJS.
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%#

# the final executable must be linked in this exact order
OBJ_LINK_LIST := $(addprefix $(TRGT_ARCH)/, crt0.o crtend.o) \
	$(OBJS) $(addprefix $(TRGT_ARCH)/, cpu.o lib_init.o crtbegin.o)

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%#
# @TODO: Fix ld include error. ld expects to find crtend.o and crtbegin.o
# to be relative to the working directory. It fails when given a relative path.
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%#

$(FATBIN): $(OBJ_LINK_LIST)
	cd $(TRGT_ARCH) && $(LD) $(LDFLAGS) crt0.o crtend.o $(OBJS) \
		cpu.o lib_init.o crtbegin.o -lgcc -o $@

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

$(ISOFILE): $(OBJ_LINK_LIST)
	grub-mkrescue -o $@ $(IMG)

#_______CLANG-FORMAT__________________________________________________________#

PHONY: format

format:
	cd $(DOCS) && find .. -type d \( -name .git -o -name tools -o -name app \) \
		-prune -o -regex '.*\.\(cc\|c\|h\)'\
		-exec clang-format-8 style=.clang-format -i {} \;

#_______CROSS-CHAIN____________________________________________________________#

.PHONY: install-cross uninstall-cross

install-cross:
	cd $(TOOLS) && bash install_cross.sh

uninstall-cross:
	@rm -rf $(TOOLS)/cross
	@mkdir $(TOOLS)/cross

#_______CLEAN ENVIRONMENT______________________________________________________#

.PHONY: clean veryclean

clean:
	@find . -depth -type d \( -name .git -o -name tools \) -prune -o -type f \
		\( -name "*.o" -o -name "*.d" \) -delete

veryclean: clean
	@rm -f .debug.lock brae.iso
	@rm -f img/boot/brae.bin img/boot/brae.bin.debug
