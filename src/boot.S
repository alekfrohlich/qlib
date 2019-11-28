.set ALIGN,    1<<0
.set MEMINFO,  1<<1
.set FLAGS,    ALIGN | MEMINFO
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

# os sections colocam a memoria em ordem?
# why align 4?
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# stack inside bss? 16 byte alligned? Does the bootloader do the skipping?
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

.section .text
.global _start
.type _start, @function
_start:

	mov $stack_top, %esp

	# turn on paging, float, etc...

	# remember to 16-byte allign.
	call kernel_main

	# infinite loop
	cli
1:	hlt
	jmp 1b

.size _start, . - _start
