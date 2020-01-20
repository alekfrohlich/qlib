#ifndef _QLIB_TRAITS_H
#define _QLIB_TRAITS_H

#include <types.h>
using namespace qlib::mediator;

template<typename T>
struct Traits {};

template<>
struct Traits<General> {
    static constexpr bool debugged = true;
};

// architecture hardware mediators
template<>
struct Traits<CPU> {
    static constexpr bool debugged = false;
};

// machine hardware mediators
template<>
struct Traits<Display> {
    static constexpr bool debugged = true;
};

template<>
struct Traits<IC> {
    static constexpr bool debugged = false;
};

#endif  // _QLIB_TRAITS_H