#include <std/ostream.h>

extern "C"
{
void _pre_kmain(void) {}

void _kmain(void) {
    using namespace std;

    cout << "eai nicolas!" << endl << "1";
}
}
