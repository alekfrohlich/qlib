# declare constants for the multiboot header
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the multiboot 'flag' field
.set MAGIC,    0x1BADB002       # magic number helps the bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# declare a header as in the multiboot standard
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# entry point
.section .text
.global _start
.type _start, @function
_start:
	# set-up stack pointer
	mov $_edata+16384, %esp

	# set-up components
	call _pre_kmain

	# call the global constructors
	call _init

	# transfer control to operating system
	call _kmain

	# call global destructors
	call _fini

	# hang if _kmain unexpectedly returns
	cli
	hlt

.size _start, . - _start
