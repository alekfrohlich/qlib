all: brae

brae:
	$(AS) boot.S -o boot.o
	$(CXX) $(CFLAGS) -c kernel.cpp -o kernel.o
	$(CXX) $(CFLAGS) -T linker.ld -o $@.bin boot.o kernel.o -lgcc
	mv $@.bin $(IMGDIR)

clean:
	@rm -f *.o *.iso