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
.globl _start
.type _start, @function
_start:
	# set-up stack pointer
	mov $_ebss+16384, %esp

	# set-up components
	call _pre_lib_init

	# call the global constructors
	call _init

	# transfer control to operating system
	call _lib_init

	# call global destructors
	call _fini

	# hang if _kmain unexpectedly returns
	cli
	hlt

.size _start, . - _start

.globl keyboard_handler
.extern keyboard_handler_main
keyboard_handler:
	call keyboard_handler_main
	iret

# %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% #
# @TODO: inline below
# %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% #

# 0x08 and 0x10 are the code and data selectors

.globl gdt_flush
gdt_flush:
	mov 4(%esp), %eax
	lgdt (%eax)
	ljmp $0x8, $reload_cs
reload_cs:
	movw $0x10, %ax
	movw %ax, %ds
	ret