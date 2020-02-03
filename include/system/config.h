#ifndef __QLIB_H
#define __QLIB_H

#define ARCH ia32
#define MACH pc

#define __ARCH_HEADER(X, ARCH) <architecture/ARCH/X.h>
#define __MACH_HEADER(X, MACH) <machine/MACH/X.h>

#define ASM __asm__ __volatile__
#define INTRIN [[gnu::always_inline, gnu::artificial]] static inline

// architecture hardware mediators
#define __CPU_H __ARCH_HEADER(cpu, ARCH)

// machine hardware mediators
#define __DISPLAY_H __MACH_HEADER(display, MACH)
#define __IC_H __MACH_HEADER(ic, MACH)

#include <utility/debug.h>

using size_t = __SIZE_TYPE__;
inline void * operator new(size_t count, void * ptr) {
    return ptr;
}

#include <system/traits.h>

#endif  // __QLIB_H
