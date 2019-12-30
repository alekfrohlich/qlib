#ifndef _QLIB_HARDWARE_VGA_H
#define _QLIB_HARDWARE_VGA_H

namespace qlib::hardware {

class Display
{
 public:
    typedef unsigned short Mode;  // foreground-background color
    typedef unsigned short Cell;  // vga entry

    static void print(const char * s);
    static void error(void);

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
    // @TODO: Maximize mechanism, minimize policy.
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    // inner workings
    static void put(char c);
    static void erase(void);
    static void scroll(void);

 private:
    // white-black and red-black operating modes
    static const Mode NORMAL_MODE = static_cast<Mode>(7 << 8);
    static const Mode ERROR_MODE = static_cast<Mode>(12 << 8);

    static const int HEIGHT = 25;
    static const int WIDTH = 80;

    static int _row;
    static int _column;

    static Mode _mode;
    static Cell * _buffer;
};

}  // namespace qlib::hardware

#endif  // _QLIB_HARDWARE_VGA_H
