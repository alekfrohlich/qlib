#include <thread.h>

// wrong but should work
static char _stack_space[1 << 16];
static char * stack1 = &_stack_space[1 << 14];
static char * stack2 = &_stack_space[1 << 15];

extern "C" int __app_entry();

namespace qlib {

Thread main_thread;
Thread other_thread;
void Thread::init() {
    db<mediator::Init>() << "Thread::init()\n";

    main_thread = Thread(stack1, __app_entry);
    other_thread = Thread(stack2, Thread::idle);
    main_thread.next = &other_thread;
    other_thread.next = &main_thread;
    Thread::running_thread = &main_thread;
}
}  // namespace qlib
