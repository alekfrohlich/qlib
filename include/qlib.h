#ifndef __QLIB_H
#define __QLIB_H

#define ARCH ia32
#define MACH pc

#define __ARCH_HEADER(X, ARCH) <architecture/ARCH/ARCH/**/_##X.h>
#define __MACH_HEADER(X, MACH) <machine/MACH/MACH     /**/_##X.h>

#include <traits.h>

#endif  // __QLIB_H
