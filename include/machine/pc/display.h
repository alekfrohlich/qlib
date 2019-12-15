#ifndef DISPLAY_H
#define DISPLAY_H

#include <std/printer.h>

class Display : public Printer
{
 public:
    typedef unsigned short Mode;  // foreground-background color
    typedef unsigned short Cell;  // vga entry

    // Printer implementation
    void print(const char * s);
    void error(void);

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
    // @TODO: Maximize mechanism, minimize policy.
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    // inner workings
    void put(char c);
    void erase(void);
    void scroll(void);

 private:
    // white-black and red-black operating modes
    static const Mode NORMAL_MODE = static_cast<Mode>(7 << 8);
    static const Mode ERROR_MODE = static_cast<Mode>(12 << 8);
    static const int HEIGHT = 25;
    static const int WIDTH = 80;

    int _row = 0;
    int _column = 0;

    Mode _mode = NORMAL_MODE;
    Cell * _buffer = reinterpret_cast<Cell *>(0xB8000);
};

extern Display display;

#endif  // DISPLAY_H