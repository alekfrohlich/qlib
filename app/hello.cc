#include <utility/ostream.h>
#include <thread.h>

int main(void) {
    using namespace qlib;

    cout << "Hello bare-metal world!\n";
    Thread::yield();
    cout << "Back at hello\n";
    while(1);

    return 0;
}