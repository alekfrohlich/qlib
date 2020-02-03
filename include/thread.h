#ifndef __QLIB_THREAD_H
#define __QLIB_THREAD_H

#include <architecture/cpu.h>

namespace qlib {

struct Thread {
    using Context = mediator::CPU::Context;

    Thread() = default;
    Thread(char * stack, int (*entry)());

    static void dispatch(Thread * prev, Thread * next);
    static void yield();
    static void exit();

    static Thread * running_thread;

    Thread * next;
    volatile Context * context;
};

};  // namespace qlib

#endif  // __QLIB_THREAD_H
