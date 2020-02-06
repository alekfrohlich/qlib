#ifndef __QLIB_THREAD_H
#define __QLIB_THREAD_H

#include <architecture/cpu.h>
#include <utility/list.h>

namespace qlib {

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
    static Thread * running() { return scheduler.chosen(); }

    void load_context() const { context->load(); }

 private:
    class Scheduler
    {
     public:
        static Thread * choose();
        static Thread * chosen();
        static void insert(Thread * thread);
        static Thread * drop_chosen();

     private:
        static inline RR_List<Thread> schedulables;
    };
    static inline Scheduler scheduler;

    volatile Context * context;
    char * stack;
};

};  // namespace qlib

#endif  // __QLIB_THREAD_H
