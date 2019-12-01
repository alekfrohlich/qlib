include $(MAKEINC)

all: image

image:
	grub-mkrescue -o brae.iso .

clean:
	@rm -f *.iso boot/*.bin
