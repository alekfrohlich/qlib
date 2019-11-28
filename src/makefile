all: brae image

brae:
	$(AS) boot.S -o boot.o
	$(CXX) $(CFLAGS) -c kernel.cpp -o kernel.o
	$(CXX) $(CFLAGS) -T linker.ld -o brae.bin boot.o kernel.o -lgcc

image:
	cp brae.bin isodir/boot/brae.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o brae.iso isodir

clean:
	@rm -f *.o *.bin *.iso