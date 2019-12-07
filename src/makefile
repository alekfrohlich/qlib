include $(MAKEINC)

.PHONY: all clean \
		arch boot machine std

all: arch boot machine std

arch:
	cd $(ARCH) && $(MAKE) all

boot:
std:
machine:

clean:
	cd $(ARCH) && $(MAKE) clean
	cd $(BOOT) && $(MAKE) clean
	cd $(MACH) && $(MAKE) clean
	cd $(STD)  && $(MAKE) clean
	cd $(TEST) && $(MAKE) clean
