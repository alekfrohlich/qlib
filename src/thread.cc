#include <system.h>
#include <thread.h>

namespace qlib {

Thread::Thread(int (*entry)()) {
    stack = new (SYSTEM_STACK) char[Traits<System>::STACK_SIZE];
    context = CPU::init_stack(stack, Thread::exit, entry);
    scheduler.insert(this);
}

void Thread::dispatch(Thread * prev, Thread * next) {
    // @TODO: add trace info here
    CPU::switch_context(
        const_cast<Context * volatile *>(&prev->context), next->context);
}

void Thread::yield() {
    Thread * prev = running();
    Thread * next = scheduler.choose();
    dispatch(prev, next);
}

void Thread::exit() {
    Thread * dying = scheduler.drop_chosen();
    Thread * next = scheduler.chosen();
    dispatch(dying, next);
};

int Thread::idle() {
    cout << "Idle\n";
    return 0;
}

Thread * Thread::Scheduler::choose() { return schedulables.choose(); }

Thread * Thread::Scheduler::chosen() { return schedulables.chosen(); }

void Thread::Scheduler::insert(Thread * thread) { schedulables.insert(thread); }

Thread * Thread::Scheduler::drop_chosen() { return schedulables.drop_chosen(); }

};  // namespace qlib