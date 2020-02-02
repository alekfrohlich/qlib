#include <architecture/cpu.h>
#include <machine/display.h>
#include <machine/ic.h>
#include <thread.h>

void other_entry_point() {
    using namespace qlib;
    using namespace qlib::mediator;

    // while (1) {
    cout << "Other\n";
    //     Thread::yield();
    // }
}

extern "C" {

extern void main(void);

qlib::Thread main_thread;
qlib::Thread other_thread;

void _lib_init(void) {
    using namespace qlib;
    using namespace qlib::mediator;

    // initialize hardware mediators
    CPU::init();
    IC::init();
    Display::init();

    // setup entry points (int still disabled)
    main_thread = Thread(stack1, 0, &other_thread);
    other_thread = Thread(stack2, other_entry_point, &main_thread);

    Thread::running_thread = &main_thread;

    // while (1) {
    cout << "Main\n";
    Thread::yield();
    // }
    cout << "Back at Main!\n";

    while (1)
        ;

    // main();
}
}
