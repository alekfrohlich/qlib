#include <system.h>
#include <thread.h>

extern "C" int __app_entry();

namespace qlib {

void Thread::init() {
    db<Init>() << "Thread::init()\n";

    // create app and idle threads
    new (SYSTEM) Thread(__app_entry);
    new (SYSTEM) Thread(Thread::idle);
}
}  // namespace qlib
