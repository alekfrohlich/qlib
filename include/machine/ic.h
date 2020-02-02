#ifndef __QLIB_MEDIATOR_IC_H
#define __QLIB_MEDIATOR_IC_H

#include <mediator.h>

// This header exports the right implementation of the Interrupt Controller
// mediator based on application traits.
#if defined(__IC_H) && !defined(__ic_common_only)
#include __IC_H
#endif

#endif  // __QLIB_MEDIATOR_IC_H
