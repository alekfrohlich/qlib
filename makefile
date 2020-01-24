SHELL := /bin/bash
include $(CURDIR)/makedefs

#________BUILD OPTIONS________________________________________________________#

.PHONY: release debug test_cli
.SILENT: release debug test_cli

# BUILD_DIR_TREE's recipe was not being executed as an order only pre-requisite
# shouldnt be dependent on order of prereqs (-j)

# strip debug symbols that came from libgcc
release: test_cli install-dirs $(BINARY) $(IMAGE)
	$(STRIP) -d $(BINARY)

debug: test_cli install-dirs $(BINARY) $(IMAGE)

test_cli:
	if [ "$(APPLICATION)" = "" ]; then \
		echo "No value for 'APPLICATION', try:"; \
		echo "$$ make APPLICATION=<app_name>"; \
		echo "" && exit 1; \
	fi

#________COMMON SOURCE CODE___________________________________________________#

SRC_DIR_TREE   := $(shell find $(SRC) -type d)
BUILD_DIR_TREE := $(subst $(SRC),$(BUILD),$(SRC_DIR_TREE))

INCLUDES := $(foreach dir, $(SRC_DIR_TREE), $(addprefix -I, $(dir)))

VPATH := $(SRC_DIR_TREE)

CXX_SRC := $(foreach dir,$(SRC_DIR_TREE),$(wildcard $(dir)/*.cc))

APP_CXX_SRC := $(wildcard $(APP)/$(APPLICATION).cc)
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

$(foreach targetdir, $(BUILD_DIR_TREE), \
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

.PHONY:install-dirs

install-dirs:
	@mkdir -p $(BUILD_DIR_TREE)

#_______PREPROCESSOR OUTPUT___________________________________________________#

.PHONY: preprocessor-output
.SILENT: preprocessor-output

preprocessor-output:
	if [ "$(FILE)" = "" ]; then \
		echo "No value for 'FILE', try:"; \
		echo "$$ make preprocessor-output FILE=<file>"; \
		echo "" && exit 1; \
	fi
	$(CXX) $(CXXFLAGS) -E $(FILE)

#_______CLANG-FORMAT__________________________________________________________#

.PHONY: format

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
