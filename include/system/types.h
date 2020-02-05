#ifndef __QLIB_TYPES_H
#define __QLIB_TYPES_H

using size_t = __SIZE_TYPE__;

enum System_Allocator { SYSTEM, SYSTEM_STACK };

inline void * operator new(size_t bytes, void * ptr) { return ptr; }
inline void * operator new[](size_t bytes, void * ptr) { return ptr; }

void * operator new(size_t, System_Allocator);
void * operator new[](size_t, System_Allocator);

#endif  // __QLIB_TYPES_H