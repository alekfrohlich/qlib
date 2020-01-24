#ifndef __QLIB_TRAITS_H
#define __QLIB_TRAITS_H

namespace qlib {

namespace mediator {
    // architecture hardware mediators
    class CPU;

    // machine hardware mediators
    class Display;
    class IC;
};  // namespace mediator

template<typename T>
struct Traits {
    static constexpr bool debugged = false;
};

};  // namespace qlib

#endif  // __QLIB_TRAITS_H
