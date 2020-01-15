#include <qlib.h>

#include <qlib/ostream.h>
#include <machine/pc/vga.h>

void main(void) {
    using namespace qlib;
    using namespace hardware;

    cout << "Hello VGA text mode!" << endl;

    while(1);
}