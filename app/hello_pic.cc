#include <ostream.h>
#include <machine/ic.h>

void main(void) {
    using namespace qlib;
    using namespace mediator;

    cout << "Hello 8259A!" << endl;
    cout << "imr = " << PIC::imr() << endl;
    cout << "irr = " << PIC::irr() << endl;
    cout << "isr = " << PIC::isr() << endl;
    while(1);
}