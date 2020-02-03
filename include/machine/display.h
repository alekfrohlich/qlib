#ifndef __QLIB_MEDIATOR_DISPLAY_H
#define __QLIB_MEDIATOR_DISPLAY_H

#include <system/config.h>

// This header exports the right implementation of the Display mediator based on
// application traits.
#if defined(__DISPLAY_H)
#include __DISPLAY_H
#endif

#endif  // __QLIB_MEDIATOR_DISPLAY_H
