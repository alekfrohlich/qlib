#include <architecture/cpu.h>
#include <machine/display.h>
#include <machine/ic.h>
#include <thread.h>

void other_entry_point() {
    using namespace qlib;
    using namespace qlib::mediator;

    while(1) {
        cout << "Other\n";
        Thread::yield();
    }
}

extern "C" {

extern void main(void);

// make gcc happy
void * __cxa_pure_virtual = 0;
int __cxa_guard_acquire(long long int *) {
    return 1;
}
void __cxa_guard_release(long long int *) {
}

void _lib_init(void) {
    using namespace qlib;
    using namespace qlib::mediator;

    // initialize hardware mediators
    CPU::init();
    IC::init();
    Display::init();

    // setup entry points (int still disabled)
    Thread::init();

    while (1) {
        cout << "Main\n";
        Thread::yield();
    }

    while (1)
        ;

    // main();
}
}
