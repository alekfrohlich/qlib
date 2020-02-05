#include <utility/ostream.h>
#include <utility/list.h>
#include <thread.h>

int main(void) {
    using namespace qlib;

    cout << "Hello bare-metal world!\n";
    Thread::yield;
    cout << "Welcome back!\n";

    List<int> list;
    for (int i = 0; i < 5; i++)
        list.push_back(new (SYSTEM) int(i));

    for (int i = 0; i < 5; i++)
        cout << "i = " << *list.pop_back() << "\n";

    while(1);

    return 0;
}