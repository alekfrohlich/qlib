#ifndef __QLIB_MEDIATOR_IC_H
#define __QLIB_MEDIATOR_IC_H

#include <system/config.h>

// This header exports the right implementation of the Interrupt Controller
// mediator based on application traits.
#if defined(__IC_H)
#include __IC_H
#endif

#endif  // __QLIB_MEDIATOR_IC_H
