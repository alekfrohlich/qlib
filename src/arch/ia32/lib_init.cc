#include <app/config.h>

extern "C" {
void _pre_lib_init(void) {
}

void _lib_init(void) {
    main();
}
}
