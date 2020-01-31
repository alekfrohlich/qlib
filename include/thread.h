#ifndef __QLIB_THREAD_H
#define __QLIB_THREAD_H

#include <architecture/cpu.h>

// extern "C" switch_context(CPU::Context & o, CPU::Context & n);
extern void other_entry_point();

static char stack1[1 << 14];
static char stack2[1 << 14];

namespace qlib {

struct Thread {
    using Reg32 = mediator::CPU::Reg32;
    using Log_Address = mediator::CPU::Log_Address;
    // using Context = mediator::CPU::Context;
    using Entry_Point = void (*)();

    Thread() = default;
    Thread(Entry_Point main, char * stack, Thread * next)
        : eip(reinterpret_cast<Log_Address>(main)),
          esp(reinterpret_cast<Log_Address>(stack)), ebp(0), next(next) {}

    static void init() {
        static Thread main_thread;
        static Thread other_thread;

        main_thread = Thread(0x0, stack1, &other_thread);
        other_thread = Thread(other_entry_point, stack2, &main_thread);

        running_thread = &main_thread;
    }

    static void switch_to(Thread * from, Thread * to);

    static void yield() {
        Thread * last = running_thread;
        running_thread = running_thread->next;

        // bool is_spwaning = running_thread->spawning;
        // if (is_spwaning)
        //     running_thread->spawning = false;

        // mediator::CPU::switch_context(last->context, running_thread->context, is_spwaning);
        switch_to(last, running_thread);
    }

    // @TODO: remove thread from sched queue and make it a member function
    static void exit() { yield(); }

    static inline Thread * running_thread = nullptr;

    Reg32 ebp;
    Reg32 eip;
    Reg32 esp;
    Thread * next;
    // Context context;
};

};  // namespace qlib

#endif  // __QLIB_THREAD_H