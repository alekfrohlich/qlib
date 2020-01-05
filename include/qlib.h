#ifndef _QLIB_H
#define _QLIB_H

/*_______ASPECTS_____________________________________________________________*/

/*_______CONFIG______________________________________________________________*/

#define ASM __asm__ __volatile__
#define INTRIN [[gnu::always_inline, gnu::artificial]] static inline

#define ARCH ia32
#define MACH pc

#define __ARCH_HEADER(X, ARCH) <arch/ARCH/X.h>
#define __MACH_HEADER(X, MACH) <machine/MACH/X.h>

#if ARCH == ia32
#    define __print_h __MACH_HEADER(vga, MACH)
#    define __cpu_h __ARCH_HEADER(cpu, ARCH)
#else
#    define __print_h __MACH_HEADER(serial, MACH)
#endif

#endif  // _QLIB_H