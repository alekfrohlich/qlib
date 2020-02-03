#ifndef __QLIB_H
#define __QLIB_H

#define ARCH ia32
#define MACH pc

#define __ARCH_HEADER(X, ARCH) <architecture/ARCH/ARCH/**/_##X.h>
#define __MACH_HEADER(X, MACH) <machine/MACH/MACH     /**/_##X.h>

#include <memory_map.h>

using size_t = __SIZE_TYPE__;
inline void * operator new(size_t count, void * ptr) {
    return ptr;
}

static struct Sys_Alloc { unsigned long brk; } SYS {SYS_HEAP};

inline void * operator new(size_t count, Sys_Alloc & alloc) {
    auto old = alloc.brk;
    alloc.brk += count;
    return reinterpret_cast<void *>(old);
}

#include <traits.h>

#endif  // __QLIB_H
