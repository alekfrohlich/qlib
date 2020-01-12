#ifndef _QLIB_HARDWARE_VGA_H
#define _QLIB_HARDWARE_VGA_H

namespace qlib::hardware {

// VGA controller
class Display
{
 public:
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
    // (foreground-background): white-black and red-black operating modes
    enum Mode {
        NORMAL = 7 << 8,
        ERROR = 12 << 8,
    };

    static constexpr int HEIGHT = 25;
    static constexpr int WIDTH = 80;

    static inline int row = 0;
    static inline int column = 0;

    inline static Mode mode = NORMAL;
    inline static Cell * buffer = reinterpret_cast<Cell *>(0xB8000);
};

}  // namespace qlib::hardware

#endif  // _QLIB_HARDWARE_VGA_H
