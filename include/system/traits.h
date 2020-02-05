#ifndef __QLIB_TRAITS_H
#define __QLIB_TRAITS_H

namespace qlib {

template<typename T>
struct Traits {
    static constexpr bool debugged = false;
    static constexpr bool enabled = true;
};

class Init;
class Application;
class System;
// architecture hardware mediators
class CPU;

// machine hardware mediators
class Machine;
class Display;
class IC;

template<>
struct Traits<Init> {
    static constexpr bool debugged = false;
};

template<>
struct Traits<Machine> {};

template<>
struct Traits<System> {
    static constexpr int STACK_SIZE = 16 * 1024;
    static constexpr int HEAP_SIZE = 16 * 1024 * 1024;
};
};  // namespace qlib

#endif  // __QLIB_TRAITS_H
