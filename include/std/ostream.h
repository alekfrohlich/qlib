#ifndef OSTREAM_H
#define OSTREAM_H

#include <std/printer.h>

namespace std {

class OStream {
  public:
    struct Endl {};
    struct Hex {};
    struct Dec {};
    struct Oct {};
    struct Bin {};
    struct Err {};

    //@TODO: instantiate cout
	OStream(Printer & printer) : _base(10), _printer(printer) {}

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
    void print(const char * s) { _printer.print(s); }
    void error(void) { _printer.error(); }
	int itoa(int v, char * s);
	int utoa(unsigned int v, char * s, unsigned int i = 0);
	int llitoa(long long int v, char * s);
	int llutoa(unsigned long long int v, char * s, unsigned int i = 0);
	int ptoa(const void * p, char * s);

	int _base;
    //@TODO: fix printer
    Printer & _printer;
    static const char _digits[];
};

extern OStream::Endl endl;
extern OStream::Hex hex;
extern OStream::Dec dec;
extern OStream::Oct oct;
extern OStream::Bin bin;

//@TODO: handle especial colors for cerr
extern OStream cout;

} // namespace std

#endif // OSTREAM_H
