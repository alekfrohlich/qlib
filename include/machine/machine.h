#ifndef __QLIB_MEDIATOR_MACHINE_H
#define __QLIB_MEDIATOR_MACHINE_H

#include <system/config.h>

// This header exports the right implementation of the Machine
// mediator based on application traits.
#if defined(__MACHINE_H)
#include __MACHINE_H
#endif

#endif  // __QLIB_MEDIATOR_MACHINE_H
