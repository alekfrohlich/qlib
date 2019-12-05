include $(MAKEINC)

.PHONY: all \
		architecture boot std

all: architecture boot std

architecture:
	cd $(ARCH) && $(MAKE) all

boot:
std:

clean:
	cd $(ARCH) && $(MAKE) clean
	cd $(LIB) && $(MAKE) clean
