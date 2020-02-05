#include <system.h>
#include <thread.h>

extern "C" int __app_entry();

namespace qlib {

void Thread::init() {
    db<Init>() << "Thread::init()\n";

    Thread * main_thread = new (SYSTEM) Thread(__app_entry);
    Thread * other_thread = new (SYSTEM) Thread(Thread::idle);
    main_thread->next = other_thread;
    other_thread->next = main_thread;
    Thread::running_thread = main_thread;
}
}  // namespace qlib
