#include <system/config.h>
#include <thread.h>

namespace qlib {

Thread * Thread::running_thread = nullptr;

Thread::Thread(char * stack, int (*entry)()) {
    context = mediator::CPU::init_stack(stack, Thread::exit, entry);
}

void Thread::dispatch(Thread * prev, Thread * next) {
    // @TODO: add trace info here
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

int Thread::idle() {
    cout << "Idle\n";
    return 0;
}

};  // namespace qlib