#ifndef _QLIB_MEDIATOR_PC_DISPLAY_H
#define _QLIB_MEDIATOR_PC_DISPLAY_H

#include <arch/cpu.h>

namespace qlib::mediator {

// IBM's VGA Controller (Text Mode)
class VGA
{
 public:
    using Cell = unsigned short;

    enum {
        TEXT_MODE_BASE = 0xB8000,
    };

    enum Color {
        BLACK = 0,
        WHITE = 15,
    };

    static constexpr int HEIGHT = 25;
    static constexpr int WIDTH = 80;

    /*________CRT GROUP OF REGISTERS_____________________________________________*/

    // The CRT group of 8-bit registers are indexed throught the CRT_ADDRESS I/O
    // Port. To read/write a data register from the group:
    //    1. Output the index of the desired Data Register to the Address Register.
    //    2. Read the value of the Data Register and save it for later restoration
    //       upon termination, if needed.
    //    3. If writing, modify the value read in step 3, making sure to mask off
    //       bits not being modified.
    //    4. If writing, write the new value from step 4 to the Data register.

    // CRT Ports could be at 0x3B4/5 depending on I/OAS Field
    enum {
        CRT_ADDRESS = 0x3D4,
        CRT_DATA = 0x3D5,
    };

    enum CRT {
        HORIZONTAL_TOTAL = 0,
        HORIZONTAL_DISPLAY_END,
        HORIZONTAL_BLANKING_START,
        HORIZONTAL_BLANKING_END,
        HORIZONTAL_RETRACE_START,
        HORIZONTAL_RETRACE_END,
        VERTICAL_TOTAL,
        OVERFLOW,
        PRESET_ROW_SCAN,
        MAX_SCAN_LINE,
        CURSOR_START,
        CURSOR_END,
        START_ADDRESS_HIGH,
        START_ADDRESS_LOW,
        CURSOR_LOCATION_HIGH,
        CURSOR_LOCATION_LOW,
        VERTICAL_RETRACE_START,
        VERTICAL_RETRACE_END,
        VERTICAL_DISPLAY_END,
        OFFSET,
        UNDERLINE_LOCATION,
        VERTICAL_BLANKING_START,
        VERTICAL_BLANKING_END,
        CRTC_MODE_CONTROL,
        LINE_COMPARE,
    };

    // CD [5] | Cursor Scan Line Start [4:0]
    enum Cursor_Start {
        CD = 1 << 5,  // Cursor Disable
    };

    static CPU::Reg8 crt_reg(CRT reg) {
        CPU::out8(CRT_ADDRESS, reg);
        return CPU::in8(CRT_DATA);
    }

    //========INITIALIZE=========================================================//
    //
    //===========================================================================//

    static void init(void);

    static void put(char c, int pos = row * WIDTH + column,
        Color foreground = WHITE, Color background = BLACK) {
        auto cell_color = (foreground) | (background << 4);
        buffer[pos] = c | (cell_color << 8);
    }

    /*________HARDWARE CURSOR________________________________________________*/

    static void disable_cursor(void) {
        CPU::out8(CRT_ADDRESS, CRT::CURSOR_START);
        CPU::out8(CRT_DATA, Cursor_Start::CD);
    }

    static void move_cursor(int pos = row * WIDTH + column) {
        CPU::out8(CRT_ADDRESS, CRT::CURSOR_LOCATION_LOW);
        CPU::out8(CRT_DATA, pos & 0xff);
        CPU::out8(CRT_ADDRESS, CRT::CURSOR_LOCATION_HIGH);
        CPU::out8(CRT_DATA, pos >> 8);
    }

 protected:
    static inline Cell * buffer = reinterpret_cast<Cell *>(TEXT_MODE_BASE);
    // software cache of hardware cursor
    static inline int row = 0, column = 0;
};

// Display mediator
class Display : VGA
{
 public:
    static void print(const char * s) {
        for (unsigned i = 0; s[i]; i++)
            put(s[i]);
        move_cursor();
    }

    static void clear(void) {
        for (int pos = 0; pos < HEIGHT * WIDTH; pos++)
            VGA::put(' ', pos);
        column = 0;
        row = 0;
    }

    static void scroll(void) {
        Cell cell;
        row--;
        column = 0;

        // move up
        for (int i = 0; i < HEIGHT - 1; i++) {
            for (int j = 0; j < WIDTH; j++) {
                cell = buffer[(i + 1) * WIDTH + j];
                buffer[i * WIDTH + j] = cell;
            }
        }

        // clear last row
        for (int j = 0; j < WIDTH; j++)
            VGA::put(' ', row * WIDTH + j);
    }

 private:
    static void put(char c) {
        if (row == HEIGHT) scroll();

        switch (c) {
            case '\n':
                column = 0;
                row++;
                return;
            case '\t':
                column += 4;
                break;
            default:
                VGA::put(c);
                break;
        }
        if (++column == WIDTH) {
            column = 0;
            row++;
        }
    }
};

}  // namespace qlib::mediator

#endif  // _QLIB_MEDIATOR_PC_DISPLAY_H
