#ifndef __QLIB_DEBUG_H
#define __QLIB_DEBUG_H

#include <system/traits.h>
#include <utility/ostream.h>

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
class Enable_Debug : public Debug
{};
template<>
class Enable_Debug<false> : public Null_Debug
{};

// could be extended on a level basis (info-log-warn-error ...)
template<typename T>
inline Enable_Debug<Traits<T>::debugged> db(void) {
    return Enable_Debug<Traits<T>::debugged>();
}

}  // namespace qlib

#endif  // __QLIB_DEBUG_H
