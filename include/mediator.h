#ifndef __QLIB_MEDIATOR_H
#define __QLIB_MEDIATOR_H

#include <qlib.h>

#define ASM __asm__ __volatile__
#define INTRIN [[gnu::always_inline, gnu::artificial]] static inline

// architecture hardware mediators
#define __CPU_H __ARCH_HEADER(cpu, ARCH)

// machine hardware mediators
#define __DISPLAY_H __MACH_HEADER(display, MACH)
#define __IC_H __MACH_HEADER(ic, MACH)

// refactor into mediator header
#include <debug.h>

#endif  // __QLIB_MEDIATOR_H