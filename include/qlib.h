#ifndef __QLIB_H
#define __QLIB_H

#define ASM __asm__ __volatile__
#define INTRIN [[gnu::always_inline, gnu::artificial]] static inline

#define ARCH ia32
#define MACH pc

#define __ARCH_HEADER(X, ARCH) <arch/ARCH/ARCH   /**/_##X.h>
#define __MACH_HEADER(X, MACH) <machine/MACH/MACH/**/_##X.h>

#include <traits.h>

// architecture hardware mediators
#define __CPU_H __ARCH_HEADER(cpu, ARCH)

// machine hardware mediators
#define __DISPLAY_H __MACH_HEADER(display, MACH)
#define __IC_H __MACH_HEADER(ic, MACH)

// refactor into mediator header
#include <debug.h>

#endif  // __QLIB_H