#include <system.h>
#include <thread.h>

namespace qlib::mediator {
void System::init() {
    if (Traits<Thread>::enabled) Thread::init();
}

};  // namespace qlib::mediator
