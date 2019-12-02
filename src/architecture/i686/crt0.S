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

# reserve a stack for the initial thread
.section .stack
.align 16
__stack_bottom:
.skip 16384 # 16 KiB
__stack_top:

# entry point
.section .text
.global _start
.type _start, @function
_start:
	mov $__stack_top, %esp

	# call the global constructors
	call _init

	# transfer control to operating system
	call kernel_main

	# hang if kernel_main unexpectedly returns
	cli
1:	hlt
	jmp 1b

.size _start, . - _start
