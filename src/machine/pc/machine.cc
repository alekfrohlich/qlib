#include <machine.h>

namespace qlib::mediator {
void Machine::init() {
    db<Init>() << "Machine::init()\n";

    if (Traits<IC>::enabled) IC::init();

    if (Traits<Display>::enabled) Display::init();
}

}  // namespace qlib::mediator
