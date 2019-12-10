#include <machine/pc/display.h>

#include <std/ostream.h>

namespace std {

OStream cout(_setup::printer);

/*________CONTROL OVERLOADS__________________________________________________*/

OStream & OStream::operator<<(const Endl & endl) {
    print("\n");
    _base = 10;
    return *this;
}

OStream & OStream::operator<<(const Hex & hex) {
    _base = 16;
    return *this;
}

OStream & OStream::operator<<(const Dec & dec) {
    _base = 10;
    return *this;
}

OStream & OStream::operator<<(const Oct & oct) {
    _base = 8;
    return *this;
}

OStream & OStream::operator<<(const Bin & bin) {
    _base = 2;
    return *this;
}

OStream & OStream::operator<<(const Err & err) {
    error();
    return *this;
}

/*________DATA OVERLOADS_____________________________________________________*/

OStream & OStream::operator<<(char c) {
    char buf[2];
    buf[0] = c;
    buf[1] = '\0';
    print(buf);
    return *this;
}

OStream & OStream::operator<<(unsigned char c) {
    return operator<<(static_cast<unsigned int>(c));
}

OStream & OStream::operator<<(int i) {
    char buf[16];
    buf[itoa(i, buf)] = '\0';
    print(buf);
    return *this;
}

OStream & OStream::operator<<(short s) {
    return operator<<(static_cast<int>(s));
}

OStream & OStream::operator<<(long l) {
    return operator<<(static_cast<int>(l));
}

OStream & OStream::operator<<(unsigned int u) {
    char buf[64];
    buf[utoa(u, buf)] = '\0';
    print(buf);
    return *this;
}

OStream & OStream::operator<<(unsigned short s) {
    return operator<<(static_cast<unsigned int>(s));
}

OStream & OStream::operator<<(unsigned long l) {
    return operator<<(static_cast<unsigned int>(l));
}

OStream & OStream::operator<<(long long int u) {
    char buf[64];
    buf[llitoa(u, buf)] = '\0';
    print(buf);
    return *this;
}

OStream & OStream::operator<<(unsigned long long int u) {
    char buf[64];
    buf[llutoa(u, buf)] = '\0';
    print(buf);
    return *this;
}

OStream & OStream::operator<<(const void * p) {
    char buf[64];
    buf[ptoa(p, buf)] = '\0';
    print(buf);
    return *this;
}

OStream & OStream::operator<<(const char * s) {
    print(s);
    return *this;
}

/*________CONVERSION FUNCTIONS_______________________________________________*/

const char OStream::_digits[] = "0123456789abcdef";

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
        if (_base == 8 || _base == 16) s[i++] = '0';
        if (_base == 16) s[i++] = 'x';
    }

    for (j = v; j != 0; i++, j /= _base)
        ;
    for (j = 0; v != 0; j++, v /= _base)
        s[i - 1 - j] = _digits[v % _base];

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
        if (_base == 8 || _base == 16) s[i++] = '0';
        if (_base == 16) s[i++] = 'x';
    }

    for (j = v; j != 0; i++, j /= _base)
        ;
    for (j = 0; v != 0; j++, v /= _base)
        s[i - 1 - j] = _digits[v % _base];

    return i;
}

int OStream::ptoa(const void * p, char * s) {
    unsigned int j, v = reinterpret_cast<unsigned int>(p);

    s[0] = '0';
    s[1] = 'x';

    for (j = 0; j < sizeof(void *) * 2; j++, v >>= 4)
        s[2 + sizeof(void *) * 2 - 1 - j] = _digits[v & 0xf];

    return j + 2;
}
}  // namespace std
