#include <app/config.h>
#include <arch/ia32/cpu.h>
#include <machine/pc/keyboard.h>
#include <machine/pc/pic.h>

extern "C" {

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
    using namespace qlib::hardware;

    CPU::default_init();
    PIC::default_init();
    Keyboard::default_init();
    PIC::unmask(PIC::KEYBOARD_LINE);

    main();
}
}
