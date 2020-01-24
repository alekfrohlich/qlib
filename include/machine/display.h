#ifndef __QLIB_MEDIATOR_DISPLAY_H
#define __QLIB_MEDIATOR_DISPLAY_H

#include <mediator.h>

// This header exports the right implementation of the Display mediator based on
// application traits.
#if defined(__DISPLAY_H) && !defined(__display_common_only)
#    include __DISPLAY_H
#endif

#endif  // __QLIB_MEDIATOR_DISPLAY_H
