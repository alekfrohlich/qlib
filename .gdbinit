target remote:1234
symbol-file img/boot/brae.bin
b _start
display /4i$pc
c
