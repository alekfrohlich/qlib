#include <app/config.h>

extern "C" {
void * __cxa_pure_virtual = 0;

void _pre_lib_init(void) {
}

void _lib_init(void) {
    main();
}
}
