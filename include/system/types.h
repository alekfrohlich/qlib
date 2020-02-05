#ifndef __QLIB_TYPES_H
#define __QLIB_TYPES_H

using size_t = __SIZE_TYPE__;

enum System_Allocator { SYSTEM, SYSTEM_STACK };

inline void * operator new(size_t bytes, void * ptr) { return ptr; }
inline void * operator new[](size_t bytes, void * ptr) { return ptr; }

// System's friend void * ::operator new() needs a prior declaration (see system.h)
inline void * operator new(size_t, System_Allocator);
inline void * operator new[](size_t, System_Allocator);

#endif  // __QLIB_TYPES_H