SHELL := /bin/bash

#_______INCLUDES______________________________________________________________#

export MAKEINC = $(CURDIR)/makeinc
include $(MAKEINC)

#_______SOURCE CODE___________________________________________________________#

# @DEBATE: how to manage two binaries without recompiling the debug version?

.SILENT: all debug
.PHONY: all debug

all: CXXFLAGS += -O2
all:
	$(MAKE) $(BINARY)
	$(MAKE) $(IMAGE)
	# delete debug symbols that came from libgcc.a and qlib
	strip -d $(BINARY)

debug: CXXFLAGS += -g
debug:
	$(MAKE) veryclean
	$(MAKE) $(BINARY)
	$(MAKE) $(IMAGE)

C_SRC 	:= $(wildcard TRGT_ARCH/*.c)

# @TODO: make app-target a cli argument

CXX_SRC :=  $(wildcard $(TRGT_MACH)/*.cc) \
	$(wildcard $(QLIB)/*.cc) \
	$(wildcard $(APP)/hello.cc)

OBJS  	:= $(CXX_SRC:.cc=.o)

# the final executable must be linked in this exact order
# (cpu.o is explicitly written here because we compile ARCH/ file by file)
OBJ_LINK_LIST := $(addprefix $(TRGT_ARCH)/, crt0.o crtend.o) \
	$(OBJS) $(addprefix $(TRGT_ARCH)/, cpu.o lib_init.o crtbegin.o)

# if the path of crtend/begin.o is not specified correctly ld will try to
# include it's own version of the files leading to strange errors
$(BINARY): $(OBJ_LINK_LIST)
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

$(IMAGE): $(OBJ_LINK_LIST)
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
	@rm -f bootable_app.iso
	@rm -f img/boot/runnable_app.bin
