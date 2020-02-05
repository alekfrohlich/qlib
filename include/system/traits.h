#ifndef __QLIB_TRAITS_H
#define __QLIB_TRAITS_H

namespace qlib {

template<typename T>
struct Traits {
    static constexpr bool debugged = false;
    static constexpr bool enabled = true;
};

// @TODO: fix namespace
namespace mediator {
    class Init;
    // architecture hardware mediators
    class CPU;

    // machine hardware mediators
    class Display;
    class IC;

};  // namespace mediator

template<>
struct Traits<mediator::Init> {
    static constexpr bool debugged = false;
};
};  // namespace qlib

#endif  // __QLIB_TRAITS_H
