#include <qlib.h>

#include <qlib/ostream.h>
#include __cpu_h

// void main(void) {
//     using namespace qlib;
//     using namespace hardware;

//     cout << "Hello segment registers!\n";

//     cout << "cs = " << CPU::cs() << "\n";
//     cout << "ds = " << CPU::ds() << "\n";
//     cout << "es = " << CPU::es() << "\n";
//     cout << "fs = " << CPU::fs() << "\n";
//     cout << "gs = " << CPU::gs() << "\n";
//     cout << "ss = " << CPU::ss() << "\n";

//     unsigned short * limit;
//     unsigned long * base;

//     CPU::gdtr(limit, base);
//     cout << "limit = " << *limit << " base = " << *base << "\n";

//     CPU::idtr(limit, base);
//     cout << "limit = " << *limit << " base = " << *base << "\n";

//     while (true);
// }