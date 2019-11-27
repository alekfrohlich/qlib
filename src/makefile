.PHONY: all brae image clean

all: brae image

brae:
	i686-elf-as boot.S -o boot.o
	i686-elf-g++ -c kernel.cpp -o kernel.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
	i686-elf-g++ -T linker.ld -o brae.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc

image:
	cp brae.bin isodir/boot/brae.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o brae.iso isodir

clean:
	@rm -f *.o *.bin *.iso