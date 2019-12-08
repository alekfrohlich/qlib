export MAKEINC = $(CURDIR)/makeinc
include $(MAKEINC)

.PHONY: all brae image clean distclean format tools

all: brae image

brae:
	cd $(SRC) && $(MAKE) all

image:
	cd $(IMG) && $(MAKE) all

format:
	cd $(DOCS) && find .. -regex '.*\.\(cc\|c\|h\)'\
		 -exec clang-format-8 style=.clang-format -i {} \;

#@TODO: add .PHONY bash script to automate tool installation
tools:

clean:
	@find . -type f -name '*.o' -delete

distclean: clean
	cd $(IMG) && $(MAKE) clean