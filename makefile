export MAKEINC = $(CURDIR)/makeinc
include $(MAKEINC)

.PHONY: all brae image clean format

all: brae image

brae:
	cd $(SRC) && $(MAKE) all

image:
	cd $(IMG) && $(MAKE) all

format:
	find . -regex '.*\.\(cc\|c\|h\)' -exec clang-format style=$(DOCS)/.clang-format -i {}  2> /dev/null \;

clean:
	cd $(APP) && $(MAKE) clean
	cd $(SRC) && $(MAKE) clean
	cd $(IMG) && $(MAKE) clean
