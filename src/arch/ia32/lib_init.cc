#include <app/config.h>
#include <arch/ia32/cpu.h>

extern "C" {

// make gcc happy
void * __cxa_pure_virtual = 0;
int __cxa_guard_acquire(long long int *) {
}
void __cxa_guard_release(long long int *) {
}

void _pre_lib_init(void) {
}

void _lib_init(void) {
    qlib::hardware::CPU::default_init();
    main();
}
}
