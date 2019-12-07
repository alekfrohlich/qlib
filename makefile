export MAKEINC = $(CURDIR)/makeinc
include $(MAKEINC)

.PHONY: all brae image clean format

all: brae image

brae:
	cd $(SRC) && $(MAKE) all

image:
	cd $(IMG) && $(MAKE) all

format:
	cd $(DOCS) && $(MAKE) format

clean:
	cd $(APP) && $(MAKE) clean
	cd $(SRC) && $(MAKE) clean
	cd $(IMG) && $(MAKE) clean
