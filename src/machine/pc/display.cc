#include <machine/display.h>

namespace qlib::mediator {

extern "C" void __Puts(const char * s) {
    Display::print(s);
};

/*________INITIALIZE HARDWARE________________________________________________*/

void VGA::init(void) {
    db<Display>() << "Cursor Location = "
                  << ((crt_reg(CRT::CURSOR_LOCATION_HIGH) << 8) |
                         crt_reg(CRT::CURSOR_LOCATION_LOW))
                  << "\n"
                  << "Cursor Enabled? = "
                  << (static_cast<bool>(
                          crt_reg(CRT::CURSOR_START) & Cursor_Start::CD) ?
                             "no" :
                             "yes")
                  << "\n";
}

}  // namespace qlib::mediator
