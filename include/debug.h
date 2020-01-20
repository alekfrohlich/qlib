#ifndef _QLIB_DEBUG_H
#define _QLIB_DEBUG_H

#include <ostream.h>
#include <traits.h>

namespace qlib {

class Debug
{
 public:
    template<typename T>
    Debug & operator<<(T p) {
        cout << p;
        return *this;
    };
};

class Null_Debug
{
 public:
    template<typename T>
    constexpr Null_Debug & operator<<(const T & o) {
        return *this;
    }

    template<typename T>
    constexpr Null_Debug & operator<<(const T * o) {
        return *this;
    }
};

template<bool Enable>
class Enable_Debug: public Debug {};
template<>
class Enable_Debug<false>: public Null_Debug {};

// could be extended on a pair-mediator base (using template and traits)
// and on a level basis (info-log-warn-error ...)
INTRIN
Enable_Debug<Traits::debugged>
db(void) {
    return Enable_Debug<Traits::debugged>();
}

}  // namespace qlib

#endif