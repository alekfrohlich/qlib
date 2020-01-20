#include <machine/display.h>

namespace qlib::mediator {

extern "C" void __Puts(const char * s) {
    Display::print(s);
};

/*________INITIALIZE HARDWARE________________________________________________*/

void VGA::init(void) {
    // disable_cursor();
}

}  // namespace qlib::mediator
