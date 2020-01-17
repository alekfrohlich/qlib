#ifndef _QLIB_H
#define _QLIB_H

#define ASM __asm__ __volatile__
#define INTRIN [[gnu::always_inline, gnu::artificial]] static inline

#define ARCH ia32
#define MACH pc

#define __ARCH_HEADER(X, ARCH) <arch/ARCH/X.h>
#define __MACH_HEADER(X, MACH) <machine/MACH/X.h>

// machine model
#define __CPU_H __ARCH_HEADER(cpu, ARCH)

// hardware mediators
#define __DISPLAY_H __MACH_HEADER(display, MACH)
#define __IC_H __MACH_HEADER(ic, MACH)

#endif  // _QLIB_H