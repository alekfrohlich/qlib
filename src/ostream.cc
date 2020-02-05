#include <utility/ostream.h>

namespace qlib {

static constexpr char DIGITS[] = "0123456789abcdef";

int OStream::itoa(int v, char * s) {
    unsigned int i = 0;

    if (v < 0) {
        v = -v;
        s[i++] = '-';
    }

    return utoa(static_cast<unsigned int>(v), s, i);
}

int OStream::utoa(unsigned int v, char * s, unsigned int i) {
    unsigned int j;

    if (!v) {
        s[i++] = '0';
        return i;
    }

    if (v > 256) {
        if (base == 8 || base == 16)
            s[i++] = '0';
        if (base == 16)
            s[i++] = 'x';
    }

    for (j = v; j != 0; i++, j /= base)
        ;
    for (j = 0; v != 0; j++, v /= base)
        s[i - 1 - j] = DIGITS[v % base];

    return i;
}

int OStream::llitoa(long long int v, char * s) {
    unsigned int i = 0;

    if (v < 0) {
        v = -v;
        s[i++] = '-';
    }

    return llutoa(static_cast<unsigned long long int>(v), s, i);
}

int OStream::llutoa(unsigned long long int v, char * s, unsigned int i) {
    unsigned long long int j;

    if (!v) {
        s[i++] = '0';
        return i;
    }

    if (v > 256) {
        if (base == 8 || base == 16)
            s[i++] = '0';
        if (base == 16)
            s[i++] = 'x';
    }

    for (j = v; j != 0; i++, j /= base)
        ;
    for (j = 0; v != 0; j++, v /= base)
        s[i - 1 - j] = DIGITS[v % base];

    return i;
}

int OStream::ptoa(const void * p, char * s) {
    unsigned int j, v = reinterpret_cast<unsigned int>(p);

    s[0] = '0';
    s[1] = 'x';

    for (j = 0; j < sizeof(void *) * 2; j++, v >>= 4)
        s[2 + sizeof(void *) * 2 - 1 - j] = DIGITS[v & 0xf];

    return j + 2;
}
}  // namespace qlib
