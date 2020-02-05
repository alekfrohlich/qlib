#ifndef __QLIB_SYSTEM_H
#define __QLIB_SYSTEM_H

#include <system/types.h>

namespace qlib::mediator {

class System
{
 public:
    friend void * ::operator new(size_t, System_Allocator);
    friend void * ::operator new[](size_t, System_Allocator);
    static void init();

 private:
    static inline char _pre_heap[1 << 18];
    static inline struct {
        unsigned long brk;
    } heap {reinterpret_cast<unsigned long>(_pre_heap)};
};

};  // namespace qlib::mediator

inline void * operator new(size_t bytes, System_Allocator allocator) {
    using namespace qlib::mediator;
    void * old = reinterpret_cast<void *>(System::heap.brk);
    System::heap.brk += bytes;
    return old;
}

inline void * operator new[](size_t bytes, System_Allocator allocator) {
    using namespace qlib::mediator;
    void * old = reinterpret_cast<void *>(System::heap.brk);
    System::heap.brk += bytes;
    return old;
}

#endif  // __QLIB_SYSTEM_H
