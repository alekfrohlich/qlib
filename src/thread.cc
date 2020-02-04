#include <system/config.h>
#include <thread.h>

// static struct Sys_Alloc { unsigned long brk; } SYS {0x50000000};

// inline void * operator new[](__SIZE_TYPE__ count, Sys_Alloc & alloc) {
//     auto old = alloc.brk;
//     alloc.brk += count;
//     return reinterpret_cast<void *>(old);
// }

namespace qlib {

Thread * Thread::running_thread = nullptr;

Thread::Thread(char * stack, int (*entry)()) {
    // char * stack = new (SYS) char[1 << 16];
    context = mediator::CPU::init_stack(stack, Thread::exit, entry);
}

void Thread::dispatch(Thread * prev, Thread * next) {
    // debug goes here
    mediator::CPU::switch_context(
        const_cast<Context * volatile *>(&prev->context), next->context);
}

void Thread::yield() {
    Thread * last = running_thread;
    running_thread = running_thread->next;
    dispatch(last, running_thread);
}

void Thread::exit() {
    yield();
};

};  // namespace qlib