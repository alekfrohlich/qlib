#ifndef OSTREAM_H
#define OSTREAM_H

#include <machine/pc/vga.h>
//! __FRAME__WORK

namespace qlib {

class OStream
{
 public:
    struct Endl {};
    struct Hex {};
    struct Dec {};
    struct Oct {};
    struct Bin {};
    struct Err {};

    OStream() : _base(10) {}

    // control overloads
    OStream & operator<<(const Endl & endl);
    OStream & operator<<(const Hex & hex);
    OStream & operator<<(const Dec & dec);
    OStream & operator<<(const Oct & oct);
    OStream & operator<<(const Bin & bin);
    OStream & operator<<(const Err & err);

    // data overloads
    OStream & operator<<(char c);
    OStream & operator<<(unsigned char c);
    OStream & operator<<(int i);
    OStream & operator<<(short s);
    OStream & operator<<(long l);
    OStream & operator<<(unsigned int u);
    OStream & operator<<(unsigned short s);
    OStream & operator<<(unsigned long l);
    OStream & operator<<(long long int u);
    OStream & operator<<(unsigned long long int u);
    OStream & operator<<(const void * p);
    OStream & operator<<(const char * s);

 private:
    // hardware dependency
    void print(const char * s) { hardware::Display::print(s); }
    void error(void) { hardware::Display::error(); }

    // conversion functions
    int itoa(int v, char * s);
    int utoa(unsigned int v, char * s, unsigned int i = 0);
    int llitoa(long long int v, char * s);
    int llutoa(unsigned long long int v, char * s, unsigned int i = 0);
    int ptoa(const void * p, char * s);

    static const char DIGITS[];

    int _base;
};

extern OStream::Endl endl;
extern OStream::Hex hex;
extern OStream::Dec dec;
extern OStream::Oct oct;
extern OStream::Bin bin;
extern OStream::Err err;
extern OStream cout;

}  // namespace qlib

#endif  // OSTREAM_H
