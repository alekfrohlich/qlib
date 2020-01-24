SHELL := /bin/bash

#________INCLUDES_____________________________________________________________#

export MAKEDEFS = $(CURDIR)/makedefs
include $(MAKEDEFS)

#________BUILD OPTIONS________________________________________________________#

.PHONY: release debug
.SILENT: release debug

# BUILD_DIRS's recipe was not being executed as an order only pre-requisite

# release has to delete debug symbols that come from libgcc.a
release: install-directories $(BINARY) $(IMAGE)
	$(STRIP) -d $(BINARY)

debug: install-directories $(BINARY) $(IMAGE)

#________COMMON SOURCE CODE___________________________________________________#

CXX_SRC_DIRS_ABS := \
	$(foreach dir, $(CXX_SRC_DIRS_REL), $(addprefix $(SRC)/, $(dir)))
BUILD_DIRS := \
	$(foreach dir, $(CXX_SRC_DIRS_REL), $(addprefix $(BUILD)/, $(dir)))

INCLUDES := $(foreach dir, $(CXX_SRC_DIRS_ABS), $(addprefix -I, $(dir)))

VPATH := $(CXX_SRC_DIRS_ABS)

CXX_SRC := $(foreach dir,$(CXX_SRC_DIRS_ABS),$(wildcard $(dir)/*.cc))

# better test variables
ifdef APPLICATION
	APP_CXX_SRC := $(wildcard $(APP)/$(APPLICATION))
else
	APP_CXX_SRC := $(wildcard $(APP)/hello.cc)
endif
APP_OBJS := $(APP_CXX_SRC:.cc=.o)

OBJS := $(subst $(SRC),$(BUILD),$(CXX_SRC:.cc=.o))
DEPS := $(OBJS:.o=.d)

#________CRT-STUFF____________________________________________________________#

$(BUILD)/architecture/$(TARGET)/crt0.o: $(TRGT_ARCH)/crt0.S
	$(AS) $(ASFLAGS) $< -o $@

$(BUILD)/architecture/$(TARGET)/crtbegin.o: $(TRGT_ARCH)/crtbegin.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/architecture/$(TARGET)/crtend.o: $(TRGT_ARCH)/crtend.c
	$(CC) $(CFLAGS) -c $< -o $@

#________GENERATING RULES_____________________________________________________#

define generate-rules
$(1)/%.o: %.cc
	$(CXX) $$(CXXFLAGS) -c $$(INCLUDES) -o $$@ $$< -MMD
endef

$(foreach targetdir, $(BUILD_DIRS), \
	$(eval $(call generate-rules, $(targetdir))))

#________LINKING______________________________________________________________#

# the final executable must be linked in this exact order
OBJ_LINK_LIST = $(addprefix $(BUILD)/architecture/$(TARGET)/, crt0.o crtend.o) \
	$(OBJS) $(APP_OBJS) $(addprefix $(BUILD)/architecture/$(TARGET)/, crtbegin.o)

# if the path of crtend/begin.o is not specified correctly ld will try to
# include it's own version of the files leading to strange errors
$(BINARY): $(OBJ_LINK_LIST)
	cd $(BUILD)/architecture/$(TARGET) && $(LD) $(LDFLAGS) crt0.o crtend.o $(OBJS) \
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

# avoid including compilation stuff when it's not needed
ifeq (, $(filter $(OTHER_RULES),$(MAKECMDGOALS)))
-include $(DEPS)
endif

#_______BOOTABLE GRUB IMAGE___________________________________________________#

# the image depends on grub.cfg which in turn depends on it's menuentries
# (BINARY). For some reason the rule executes every time if it depends on
# BINARY so it's left depending on the objs.

$(IMAGE): $(IMG)/boot/grub/grub.cfg $(OBJ_LINK_LIST)
	grub-mkrescue -o $@ $(IMG)

#_______SETUP ENVIRONMENT______________________________________________________#

.PHONY:install-directories

install-directories:
	@mkdir -p $(BUILD_DIRS)

#_______PREPROCESSOR OUTPUT___________________________________________________#

.PHONY: preprocessor-output

preprocessor-output:
	$(CXX) $(CXXFLAGS) -E $(FILE)

#_______CLANG-FORMAT__________________________________________________________#

PHONY: format

# clang-format-8 behaves awkwardly (when ran, always: No such file or
# directory), but seems to work nonetheless.
format:
	find . -type d \( -name .git -o -name tools -o -name app \) \
		-prune -o -regex '.*\.\(cc\|c\|h\)'\
		-exec clang-format-8 style=llvm -i {} \; 2> /dev/null

#_______CROSS-CHAIN____________________________________________________________#

.PHONY: install-cross uninstall-cross

install-cross:
	cd $(TOOLS) && bash install_cross.sh

uninstall-cross:
	rm -rf $(TOOLS)/cross
	mkdir $(TOOLS)/cross

#_______CLEAN ENVIRONMENT______________________________________________________#

.PHONY: clean veryclean

clean:
	find . -depth -type d \( -name .git -o -name tools \) -prune -o -type f \
		\( -name "*.o" -o -name "*.d" \) -delete
	rm -f bootable_app.iso
	rm -f img/boot/runnable_app.release
	rm -f img/boot/runnable_app.debug

veryclean: clean
	rm -rf build
