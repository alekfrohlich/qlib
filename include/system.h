#ifndef __QLIB_SYSTEM_H
#define __QLIB_SYSTEM_H

#include <system/config.h>

namespace qlib {

class System
{
 public:
    friend void * ::operator new(size_t, System_Allocator);
    friend void * ::operator new[](size_t, System_Allocator);
    static void init();

 private:
    static inline char _pre_heap[Traits<System>::HEAP_SIZE];
    static inline struct {
        unsigned long brk;
    } heap {reinterpret_cast<unsigned long>(_pre_heap)};
};

};  // namespace qlib

inline void * operator new(size_t bytes, System_Allocator allocator) {
    using namespace qlib;
    auto old = System::heap.brk;
    System::heap.brk += bytes;
    return reinterpret_cast<void *>(
        (allocator == SYSTEM) ? old : System::heap.brk);
}

inline void * operator new[](size_t bytes, System_Allocator allocator) {
    using namespace qlib;
    auto old = System::heap.brk;
    System::heap.brk += bytes;
    return reinterpret_cast<void *>(
        (allocator == SYSTEM) ? old : System::heap.brk);
}

#endif  // __QLIB_SYSTEM_H
