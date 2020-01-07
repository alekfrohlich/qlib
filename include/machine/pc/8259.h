#ifndef _QLIB_HARDWARE_PIC_H
#define _QLIB_HARDWARE_PIC_H

#include <arch/ia32/cpu.h>

namespace qlib::hardware {

// 8259 controller board
class PIC
{
 public:
    typedef unsigned char Mask;

    // @TODO: refactor the following enum
    // usefull values
    enum {
        MASK_ALL = 0xff,
        MASK_NONE = 0x00,
        EOI = 0x20,
        READ_IRR = 0xa,
        READ_ISR = 0xb,
    };

    enum Mode {
        MASTER,
        SLAVE,
    };

    enum IRQ {
        PIT_LINE = 1 << 0,
        KEYBOARD_LINE = 1 << 1,
        CASCADE_LINE = 1 << 2,
        COM2_LINE = 1 << 3,
        COM1_LINE = 1 << 4,
        LPT2_LINE = 1 << 5,
        FLOPPY_LINE = 1 << 6,
        SPURIUS_LINE = 1 << 7,
        RTS_LINE = 1 << 8,
        NIC_LINE = 1 << 9,
        SCSI_LINE = 1 << 10,
        FREE_LINE = 1 << 11,
        PS2_MOUSE_LINE = 1 << 12,
        FPU_LINE = 1 << 13,
        PRIMARY_ATA_LINE = 1 << 14,
        SECONDARY_ATA_LINE = 1 << 15,
    };

    // @TODO: verify the following values (compare them to the keyboard int handler)
    // PIC IO ports
    enum {
        PIC1_CMD = 0x20,
        PIC1_DATA = 0x21,
        PIC2_CMD = 0xA0,
        PIC2_DATA = 0xA1,
    };

    static Mask _pic1_mask;
    static Mask _pic2_mask;
    static unsigned constexpr LINES_PER_PIC = 8;

    //========DEFAULT INIT=======================================================//
    // Set up intel's 8259A without cascading in unbuffered, edge-triggered 8086
    // mode. Also Configure interrupt vectors to first available offset (0x20 and
    // 0x28). No auto eoi.
    //===========================================================================//

    static void default_init(void);

    INTRIN void eoi(IRQ line) {
        CPU::out8(line < LINES_PER_PIC ? PIC1_CMD : PIC2_CMD, EOI);
    }

    INTRIN CPU::Reg8 imr(Mode mode = MASTER) {
        return (mode == MASTER) ? _pic1_mask : _pic2_mask;
    }

    INTRIN CPU::Reg8 irr(Mode mode = MASTER) {
        CPU::out8((mode == MASTER) ? PIC1_CMD : PIC2_CMD, READ_IRR);
        return CPU::in8((mode == MASTER) ? PIC1_CMD : PIC2_CMD);
    }

    INTRIN CPU::Reg8 isr(Mode mode = MASTER) {
        CPU::out8((mode == MASTER) ? PIC1_CMD : PIC2_CMD, READ_ISR);
        return CPU::in8((mode == MASTER) ? PIC1_CMD : PIC2_CMD);
    }

    /*________MASK/UNMASK IRQs___________________________________________________*/

    INTRIN void mask(IRQ line) {
        if (line < LINES_PER_PIC) {
            _pic1_mask |= line;
            CPU::out8(PIC1_DATA, _pic1_mask);
        } else {
            _pic2_mask |= line;
            CPU::out8(PIC2_DATA, _pic2_mask);
        }
    }

    INTRIN void mask(Mask mask, Mode mode = MASTER) {
        if (mode == MASTER)
            _pic1_mask = mask;
        else
            _pic2_mask = mask;
    }

    INTRIN void unmask(IRQ line) {
        if (line < LINES_PER_PIC) {
            _pic1_mask &= ~(line);
            CPU::out8(PIC1_DATA, _pic1_mask);
        } else {
            _pic2_mask &= ~(line);
            CPU::out8(PIC2_DATA, _pic2_mask);
        }
    }
};

}  // namespace qlib::hardware

#endif  // _QLIB_HARDWARE_PIC_H