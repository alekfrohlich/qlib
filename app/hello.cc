#include <utility/ostream.h>
#include <utility/list.h>
#include <thread.h>

int pop_up_thread() {
    qlib::cout << "Pop up thread!\n";
    return 0;
}

int main(void) {
    using namespace qlib;

    cout << "Hello bare-metal world!\n";
    Thread::yield();
    cout << "Welcome back!\n";

    Thread pop_up(pop_up_thread);
    Thread::yield();

    while(1);

    return 0;
}