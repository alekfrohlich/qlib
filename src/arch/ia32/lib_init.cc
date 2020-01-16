#include <arch/cpu.h>
#include <machine/ic.h>
#include <machine/keyboard.h>

extern "C" {

extern void main(void);

// make gcc happy
void * __cxa_pure_virtual = 0;
int __cxa_guard_acquire(long long int *) {
    return 1;
}
void __cxa_guard_release(long long int *) {
}

void _pre_lib_init(void) {
}

void _lib_init(void) {
    using namespace qlib::mediator;

    CPU::init();
    PIC::init();
    Keyboard::init();
    VGA::init();
    PIC::unmask(PIC::KEYBOARD_LINE);

    main();
}
}
