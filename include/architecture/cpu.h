#ifndef __QLIB_MEDIATOR_CPU_H
#define __QLIB_MEDIATOR_CPU_H

#include <system/config.h>

// This header exports the right implementation of the CPU mediator based on
// application traits.
#if defined(__CPU_H)
#include __CPU_H
#endif

#endif  // __QLIB_MEDIATOR_CPU_H
