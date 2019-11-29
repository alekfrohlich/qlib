export MAKEINC = $(CURDIR)/makeinc
include $(MAKEINC)

.PHONY: all brae image clean

all: brae image

brae:
	cd $(SRC) && $(MAKE) all

image:
	cd $(IMG) && $(MAKE) all

clean:
	cd $(SRC) && $(MAKE) clean
	cd $(IMG) && $(MAKE) clean
