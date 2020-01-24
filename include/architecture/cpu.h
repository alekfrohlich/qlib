#ifndef __QLIB_MEDIATOR_CPU_H
#define __QLIB_MEDIATOR_CPU_H

#include <mediator.h>

// This header exports the right implementation of the CPU mediator based on
// application traits.
#if defined(__CPU_H) && !defined(__cpu_common_only)
#    include __CPU_H
#endif

#endif  // __QLIB_MEDIATOR_CPU_H
