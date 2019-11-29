include $(MAKEINC)

all: brae

brae:
	$(AS) boot.S -o boot.o
	$(CXX) $(CFLAGS) -c kernel.cpp -o kernel.o
	$(CXX) $(CFLAGS) -T linker.ld -o $@.bin boot.o kernel.o -lgcc
	@echo $(IMG)
	mv $@.bin $(IMG)/boot

clean:
	@rm -f *.o *.iso