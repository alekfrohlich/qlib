#ifndef __QLIB_THREAD_H
#define __QLIB_THREAD_H

#include <architecture/cpu.h>

// extern "C" switch_context(CPU::Context & o, CPU::Context & n);
extern void other_entry_point();

static char stack1[1 << 14];
static char stack2[1 << 14];

namespace qlib {

struct Thread {
    using EFlags = mediator::CPU::EFlags;
    using Reg32 = mediator::CPU::Reg32;
    using Log_Address = mediator::CPU::Log_Address;
    using Context = mediator::CPU::Context;
    using Entry_Point = void (*)();

    Thread() = default;
    // change context creation to placement new
    Thread(Entry_Point main, char * stack, Thread * next) : next(next) {
        ASM("   push %%esi          \n"
            "   mov  %%esp, %%esi   \n"
            "   mov  %0,    %%esp   \n"
            "   " ::"m"(reinterpret_cast<Log_Address>(stack))
            : "esi");

        // push Thread::exit so that the thread is automatically cleaned up
        // after finishing executing
        ASM("push %0" : : "g"(reinterpret_cast<Log_Address>(Thread::exit)));

        // construct this->context
        ASM("   push %0     " ::"m"(reinterpret_cast<Log_Address>(main)));
        ASM("   push %ebp   ");  // value doesnt matter
        ASM("   push %0     " :: "i"(EFlags::DEFAULT));
        ASM("   pusha       ");

        // update this->context
        ASM("   mov %%esp, %0" : : "m"(this->context));

        ASM("   mov  %esi, %esp    \n"
            "   pop  %esi          \n");
    }

    static void init() {
        static Thread main_thread;
        static Thread other_thread;

        main_thread = Thread(0x0, stack1, &other_thread);
        other_thread = Thread(other_entry_point, stack2, &main_thread);

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
