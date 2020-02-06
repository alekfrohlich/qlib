#include <system.h>
#include <thread.h>

namespace qlib {

Thread::Thread(int (*entry)()) {
    context =
        CPU::init_stack(new (SYSTEM_STACK) char[Traits<System>::STACK_SIZE],
            Thread::exit, entry);
    sched_list.push_front(this);
}

void Thread::dispatch(Thread * prev, Thread * next) {
    // @TODO: add trace info here
    CPU::switch_context(
        const_cast<Context * volatile *>(&prev->context), next->context);
}

void Thread::yield() {
    Thread * prev = running();
    Thread * next = choose();
    dispatch(prev, next);
}

void Thread::exit() {
    Thread * dying = running();
    Thread * next = choose();
    sched_list.pop_front();
    dispatch(dying, next);
};

int Thread::idle() {
    cout << "Idle\n";
    return 0;
}

Thread * Thread::running() { return sched_list.back(); }

Thread * Thread::choose() {
    sched_list.push_front(sched_list.pop_back());
    return sched_list.back();
}

};  // namespace qlib