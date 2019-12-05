#include <std/ostream.h>

extern "C"
{
void _pre_kmain(void) {}

void _kmain(void) {
    using namespace std;

    cout << "eai nicolas!" << 1123 << endl << hex << 0x00010000;
}
}
