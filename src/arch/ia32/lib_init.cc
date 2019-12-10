#include <app/config.h>
#include <arch/ia32/cpu.h>

extern "C" {

// make gcc happy
void * __cxa_pure_virtual = 0;

void _pre_lib_init(void) {
    CPU::init();
}

void _lib_init(void) {
    main();
}
}
