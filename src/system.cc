#include <system.h>
#include <thread.h>

namespace qlib {

extern "C" void _pre_main() {
    Thread * first_thread = Thread::running_thread;

    first_thread->context->load();
}

void System::init() {
    if (Traits<Thread>::enabled)
        Thread::init();
}

};  // namespace qlib
