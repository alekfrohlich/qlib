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

	OStream() : _error(false) {}

	OStream & operator<<(const Endl & endl) {
        _print("\n");
        return *this;
    }

	OStream & operator<<(char c) {
		_putc(c);
		return *this;
	}

	OStream & operator<<(const char * s) {
		_print(s);
		return *this;
	}

	//@TODO: add remaining overloads

  private:
	//@TODO: add type-to-string conversors

    bool _error;
};

//@TODO: add remaining control variables
extern OStream::Endl endl;

//@TODO: handle especial colors for cerr
extern OStream cout, cerr;

} // namespace std

namespace _brae {
extern std::OStream kout, kerr;
} // namespace _brae

#endif // OSTREAM_H
