#ifndef QLIB_H
#define QLIB_H

#define ASM __asm__ __volatile__

#define ARCH ia32
#define MACH pc

#define __ARCH_HEADER(X, ARCH) <arch/ARCH/X.h>
#define __MACH_HEADER(X, MACH) <machine/MACH/X.h>

#if ARCH==ia32
    #define __print_h __MACH_HEADER(vga,MACH)
#else
    #define __print_h __MACH_HEADER(serial,MACH)
#endif

#endif  // QLIB_H