SHELL := /bin/bash

#________INCLUDES_____________________________________________________________#

export MAKEINC = $(CURDIR)/makeinc
include $(MAKEINC)

#________GENERIC SOURCE CODE__________________________________________________#

.PHONY: all

CXX_SRC_DIRS_ABS := $(foreach dir, $(CXX_SRC_DIRS_REL), $(addprefix $(SRC)/, $(dir)))
BUILD_DIRS := $(foreach dir, $(CXX_SRC_DIRS_REL), $(addprefix $(BUILD)/, $(dir)))

INCLUDES := $(foreach dir, $(CXX_SRC_DIRS_ABS), $(addprefix -I, $(dir)))

VPATH := $(CXX_SRC_DIRS_ABS) $(TRGT_ARCH)

CXX_SRC := $(foreach dir,$(CXX_SRC_DIRS_ABS),$(wildcard $(dir)/*.cc))

APP_CXX_SRC := $(wildcard $(APP)/*.cc)
APP_OBJS := $(APP_CXX_SRC:.cc=.o)

OBJS := $(subst $(SRC),$(BUILD),$(CXX_SRC:.cc=.o))
DEPS := $(OBJS:.o=.d)

#________SPECIFIC SOURCE CODE_________________________________________________#

# @TODO: shouldn't install-directories be an order-only pre-req?
all: install-directories install-crt-stuff $(BINARY) $(IMAGE)

.PHONY: install-crt-stuff

install-crt-stuff:
	$(AS) $(ASFLAGS) $(addprefix $(TRGT_ARCH)/, crt0.S) -o $(addprefix $(BUILD)/arch/$(TARGET)/, crt0.o)
	$(CC) $(CFLAGS) -c $(addprefix $(TRGT_ARCH)/, crtbegin.c) -o $(addprefix $(BUILD)/arch/$(TARGET)/, crtbegin.o)
	$(CC) $(CFLAGS) -c $(addprefix $(TRGT_ARCH)/, crtend.c) -o $(addprefix $(BUILD)/arch/$(TARGET)/, crtend.o)

#________GENERATING RULES_____________________________________________________#

define generate-rules
$(1)/%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $$(INCLUDES) -o $$@ $$< -MMD
endef

$(foreach targetdir, $(BUILD_DIRS), $(eval $(call generate-rules, $(targetdir))))

#________LINKING______________________________________________________________#

# the final executable must be linked in this exact order
OBJ_LINK_LIST := $(addprefix $(BUILD)/arch/$(TARGET)/, crt0.o crtend.o) \
	$(OBJS) $(APP_OBJS) $(addprefix $(BUILD)/arch/$(TARGET)/, crtbegin.o)

$(BINARY): $(OBJ_LINK_LIST)

# if the path of crtend/begin.o is not specified correctly ld will try to
# include it's own version of the files leading to strange errors
$(BINARY): $(OBJ_LINK_LIST)
	cd $(BUILD)/arch/$(TARGET) && $(LD) $(LDFLAGS) crt0.o crtend.o $(OBJS) \
		$(APP_OBJS) crtbegin.o -lgcc -o $@


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


-include $(DEPS)

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

#_______SETUP ENVIRONMENT______________________________________________________#

.PHONY:install-directories

install-directories:
	mkdir -p $(BUILD_DIRS)

#_______CLEAN ENVIRONMENT______________________________________________________#

.PHONY: clean veryclean

clean:
	@find . -depth -type d \( -name .git -o -name tools \) -prune -o -type f \
		\( -name "*.o" -o -name "*.d" \) -delete
	@rm -f bootable_app.iso
	@rm -f img/boot/runnable_app.bin

veryclean: clean
	@rm -rf build
	@echo $(CXX_SRC_DIRS_ABS)
	@echo $(BUILD_DIRS)
	@echo $(INCLUDES)
	@echo $(VPATH)
	@echo $(CXX_SRC)
	@echo $(OBJS)
	@echo $(DEPS)
	@echo $(OBJ_LINK_LIST)
