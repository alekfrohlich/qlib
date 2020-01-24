#include <machine/display.h>

void __Puts(const char * s) {
    qlib::mediator::Display::print(s);
};

namespace qlib::mediator {

void VGA::init(void) {
    cursor(true);
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
