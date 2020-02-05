#ifndef __QLIB_H
#define __QLIB_H

#define ARCH ia32
#define MACH pc

#define __ARCH_HEADER(X, ARCH) <architecture/ARCH/X.h>
#define __MACH_HEADER(X, MACH) <machine/MACH/X.h>

#define ASM __asm__ __volatile__

// architecture hardware mediators
#define __CPU_H __ARCH_HEADER(cpu, ARCH)

// machine hardware mediators
#define __DISPLAY_H __MACH_HEADER(display, MACH)
#define __IC_H __MACH_HEADER(ic, MACH)
#define __MACHINE_H __MACH_HEADER(machine, MACH)

#include <system/traits.h>

// everyone needs
#include <utility/debug.h>
#include <utility/ostream.h>

#endif  // __QLIB_H
