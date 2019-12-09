export MAKEINC = $(CURDIR)/makeinc
include $(MAKEINC)

.PHONY: all clean format image tools

#_______SOURCE CODE___________________________________________________________#

all: $(FATBIN) image

CXX_SRC :=  $(wildcard $(TRGT_MACH)/*.cc) \
			$(wildcard $(STD)/*.cc) \
		    $(wildcard $(APP)/*.cc)

OBJS  	:= $(CXX_SRC:.cc=.o)

# the final executable must be linked in this order
#
#
OBJ_LINK_LIST := $(TRGT_ARCH)/crt0.o $(TRGT_ARCH)/crtend.o $(OBJS) $(TRGT_ARCH)/lib_init.o $(TRGT_ARCH)/crtbegin.o

# LD expects crtbegin.o, not /path/to/crtbegin.o
#
#
$(FATBIN): $(OBJ_LINK_LIST)
	cd $(TRGT_ARCH) && $(LD) $(LDFLAGS) crt0.o crtend.o $(OBJS) lib_init.o crtbegin.o -lgcc -o $(FATBIN)

%.o: %.S
	 $(AS) $(ASFLAGS) $< -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

#_______BOOTABLE GRUB IMAGE___________________________________________________#

image:
	grub-mkrescue -o $(BIN)/brae.iso $(IMG)

#_______CLANG-FORMAT__________________________________________________________#


format:
	cd $(DOCS) && find .. -regex '.*\.\(cc\|c\|h\)'\
		 -exec clang-format-8 style=.clang-format -i {} \;

#_______CROSS-CHAIN SETUP_____________________________________________________#

#@TODO: add .PHONY bash script to automate tool installation
tools:

clean:
	@find . -type f \( -name "*.o" -o -name "brae.iso" -o -name "brae.bin" \) -delete
