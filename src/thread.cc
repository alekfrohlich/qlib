#include <system.h>
#include <thread.h>

namespace qlib {

Thread * Thread::running_thread = nullptr;

Thread::Thread(int (*entry)()) {
    context =
        CPU::init_stack(new (SYSTEM_STACK) char[Traits<System>::STACK_SIZE],
            Thread::exit, entry);
}

void Thread::dispatch(Thread * prev, Thread * next) {
    // @TODO: add trace info here
    CPU::switch_context(
        const_cast<Context * volatile *>(&prev->context), next->context);
}

void Thread::yield() {
    Thread * last = running_thread;
    running_thread = running_thread->next;
    dispatch(last, running_thread);
}

void Thread::exit() { yield(); };

int Thread::idle() {
    cout << "Idle\n";
    return 0;
}

};  // namespace qlib