#ifndef __QLIB_THREAD_H
#define __QLIB_THREAD_H

#include <architecture/cpu.h>
#include <utility/list.h>

namespace qlib {

class Scheduler
{};

class Thread
{
 public:
    using Context = CPU::Context;

    Thread(int (*entry)());

    static void dispatch(Thread * prev, Thread * next);
    static void yield();
    static void exit();

    static int idle();

    static void init();
    static Thread * running();

    void load_context() const { context->load(); }

 private:
    // refactor it (and it's horrible performance) into Scheduler class
    static Thread * choose();
    static inline List<Thread> sched_list;

    volatile Context * context;
};

};  // namespace qlib

#endif  // __QLIB_THREAD_H
