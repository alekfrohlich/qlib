#include <std/printer.h>

namespace std {

//@TODO: x86-32-vga specific! move to machine/arch!
class Display : public Printer {
  public:
    void print(const char * s);
    void error(void);

  private:
    void put(char c);
    void erase(void);
    void scroll(void);

    typedef unsigned short Mode;
    typedef unsigned short Cell;

    static const Mode _normal = static_cast<Mode>(7 << 8);
    static const Mode _error  = static_cast<Mode>(12 << 8);

    int _row = 0;
    int _column = 0;
    static const int _height = 25;
    static const int _width  = 80;

    Mode _mode;
    Cell * _buffer = reinterpret_cast<Cell*>(0xB8000);
};

extern Display display;

}; // namespace std