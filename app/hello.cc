#include <qlib.h>

#include <qlib/ostream.h>
#include <machine/pc/8259.h>

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
// @TODO: Make target-app a parameter to make or a new python/bash script, else
//        the user will have to swap the contents of app/ each time he/she
//        wants to compile a new app.
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

void main(void) {
    using namespace qlib;
    using namespace hardware;

    cout << "Hello 8259A!" << endl;
    cout << "imr = " << PIC::imr() << endl;
    cout << "irr = " << PIC::irr() << endl;
    cout << "isr = " << PIC::isr() << endl;

    while(1);
}