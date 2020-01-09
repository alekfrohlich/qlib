#include <qlib.h>

#include <qlib/ostream.h>
#include <machine/pc/8259.h>

void main(void) {
    using namespace qlib;
    using namespace hardware;

    cout << "Hello 8259A!" << endl;
    cout << "imr = " << PIC::imr() << endl;
    cout << "irr = " << PIC::irr() << endl;
    cout << "isr = " << PIC::isr() << endl;

    while(1);
}