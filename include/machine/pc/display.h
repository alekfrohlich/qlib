#include <std/printer.h>

namespace std {

//@TODO: x86-32-vga specific! move to machine/arch!
class Display : public Printer
{
 public:
    // Printer implementation
    void print(const char * s);
    void error(void);

    // inner workings
    void put(char c);
    void erase(void);
    void scroll(void);

 private:
    typedef unsigned short Mode;  // foreground-background color
    typedef unsigned short Cell;  // vga entry

    // white-black and red-black operating modes
    static const Mode _normal = static_cast<Mode>(7 << 8);
    static const Mode _error = static_cast<Mode>(12 << 8);

    int _row = 0;
    int _column = 0;

    static const int _height = 25;
    static const int _width = 80;

    Mode _mode = _normal;
    Cell * _buffer = reinterpret_cast<Cell *>(0xB8000);
};

extern Display display;

};  // namespace std