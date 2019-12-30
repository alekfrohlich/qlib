#ifndef _QLIB_HARDWARE_PIC_H
#define _QLIB_HARDWARE_PIC_H

#include <arch/ia32/cpu.h>

namespace qlib::hardware {

// 8259 controller board
class PIC
{
 public:
    typedef unsigned char Mask;

    // interrupt request lines
    enum IRQ {
        KEYBOARD_LINE = 2
    };  // @TODO: should be 1, but breaks if changed.

    // interrupt lines per PIC
    static const unsigned INT_LINES = 8;

    // I/O ports
    static const CPU::IOPort PIC1_STATUS;
    static const CPU::IOPort PIC1_DATA;
    static const CPU::IOPort PIC2_STATUS;
    static const CPU::IOPort PIC2_DATA;

    // current mask
    static Mask _mask;

    static void init(void);

    /*________MANAGE IRQs________________________________________________________*/

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
    // @TODO: Make mask/unmask varargs.
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    static void mask(const IRQ & line) {
        if (line < INT_LINES)
            CPU::out8(PIC1_DATA, PIC::_mask | line);
        else
            CPU::out8(PIC2_DATA, PIC::_mask | line);
    }

    static void unmask(const IRQ & line) {
        if (line < INT_LINES)
            CPU::out8(PIC1_DATA, PIC::_mask & ~(line));
        else
            CPU::out8(PIC2_DATA, PIC::_mask & ~(line));
    }
};

}  // namespace qlib::hardware

#endif  // _QLIB_HARDWARE_PIC_H