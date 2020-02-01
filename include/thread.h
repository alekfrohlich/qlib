#ifndef __QLIB_THREAD_H
#    define __QLIB_THREAD_H

#    include <architecture/cpu.h>

// extern "C" switch_context(CPU::Context & o, CPU::Context & n);
extern int other_entry_point();

static char stack1[1 << 14];
static char stack2[1 << 14];

namespace qlib {

struct Thread {
    using EFlags = mediator::CPU::EFlags;
    using Reg32 = mediator::CPU::Reg32;
    using Log_Address = mediator::CPU::Log_Address;
    using Context = mediator::CPU::Context;

    Thread() = default;
    // change context creation to placement new
    Thread(char * stack, int (*entry)(), Thread * next) : next(next) {
        context = mediator::CPU::init_stack(
            reinterpret_cast<Log_Address>(stack), Thread::exit, entry);
    }

    static void init() {
        static Thread main_thread;
        static Thread other_thread;

        main_thread = Thread(stack1, 0, &other_thread);
        other_thread = Thread(stack2, other_entry_point, &main_thread);

        running_thread = &main_thread;
    }

    // static void switch_to(Thread * from, Thread * to);

    static void yield() {
        Thread * last = running_thread;
        running_thread = running_thread->next;

        mediator::CPU::switch_context(
            const_cast<Context * volatile *>(&last->context),
            running_thread->context);
    }

    // @TODO: remove thread from sched queue and make it a member function
    static void exit() { yield(); }

    static inline Thread * running_thread = nullptr;

    Thread * next;
    volatile Context * context;
};

};  // namespace qlib

#endif  // __QLIB_THREAD_H

// $16 = {edi = 0, esi = 1097680, _ebp = 1097704, _esp = 1070000, ebx = 65536, edx = 981, ecx = 981, eax = 0, eflags = 2, ebp = 1097704, eip = 0}
