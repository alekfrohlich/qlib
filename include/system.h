#ifndef __QLIB_SYSTEM_H
#define __QLIB_SYSTEM_H

using size_t = __SIZE_TYPE__;
namespace qlib::mediator {

class System
{
    // friend void * ::operator new(size_t, void *);
 public:
    static void init();
};

};  // namespace qlib::mediator

inline void * operator new(size_t count, void *ptr) {
    return ptr;
}

#endif  // __QLIB_SYSTEM_H
