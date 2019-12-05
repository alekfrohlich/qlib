#ifndef OSTREAM_H
#define OSTREAM_H

extern "C" {
	void _putc(const char c);
	void _print(const char * s);
}

namespace std {

class OStream {
  public:
    struct Endl {};
    struct Hex {};
    struct Dec {};
    struct Oct {};
    struct Bin {};
    struct Err {};

	OStream() : _error(false), _base(10) {}

    OStream & operator<<(const Endl & endl) {
        _print("\n");
        _base = 10;
        return *this;
    }

    OStream & operator<<(const Hex & hex) {
        _base = 16;
        return *this;
    }

    OStream & operator<<(const Dec & dec) {
        _base = 10;
        return *this;
    }

    OStream & operator<<(const Oct & oct) {
        _base = 8;
        return *this;
    }

    OStream & operator<<(const Bin & bin) {
        _base = 2;
        return *this;
    }

    OStream & operator<<(const Err & err) {
        _error = true;
        return *this;
    }

    OStream & operator<<(char c) {
        _putc(c);
        return *this;
    }

    OStream & operator<<(unsigned char c) {
        return operator<<(static_cast<unsigned int>(c));
    }

    OStream & operator<<(int i) {
        char buf[16];
        buf[itoa(i, buf)] = '\0';
        _print(buf);
        return *this;
    }

    OStream & operator<<(short s) {
        return operator<<(static_cast<int>(s));
    }

    OStream & operator<<(long l) {
        return operator<<(static_cast<int>(l));
    }

    OStream & operator<<(unsigned int u) {
        char buf[64];
        buf[utoa(u, buf)] = '\0';
        _print(buf);
        return *this;
    }

    OStream & operator<<(unsigned short s) {
        return operator<<(static_cast<unsigned int>(s));
    }

    OStream & operator<<(unsigned long l) {
        return operator<<(static_cast<unsigned int>(l));
    }

    OStream & operator<<(long long int u) {
        char buf[64];
        buf[llitoa(u, buf)] = '\0';
        _print(buf);
        return *this;
    }

    OStream & operator<<(unsigned long long int u) {
        char buf[64];
        buf[llutoa(u, buf)] = '\0';
        _print(buf);
        return *this;
    }

    OStream & operator<<(const void * p) {
        char buf[64];
        buf[ptoa(p, buf)] = '\0';
        _print(buf);
        return *this;
    }

    OStream & operator<<(const char * s) {
        _print(s);
        return *this;
    }

  private:
	int itoa(int v, char * s);
	int utoa(unsigned int v, char * s, unsigned int i = 0);
	int llitoa(long long int v, char * s);
	int llutoa(unsigned long long int v, char * s, unsigned int i = 0);
	int ptoa(const void * p, char * s);

	int _base;
    bool _error;
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
