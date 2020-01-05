#include <arch/ia32/cpu.h>
#include <machine/pc/8259.h>

namespace qlib::hardware {

/*________INITIALIZE HARDWARE________________________________________________*/

PIC::Mask PIC::_pic1_mask = PIC::MASK_NONE;
PIC::Mask PIC::_pic2_mask = PIC::MASK_NONE;

void PIC::default_init(void) {
    // ICW1, initialization:
    // 1 | edge triggered | interval of 8 | not single | ICW4 needed
    CPU::out8(0x20, 0x11);
    CPU::out8(0xA0, 0x11);

    // ICW2, mapping configuration:
    // PIC1: 0x20-0x27, PIC2: 0x28-0x2f
    CPU::out8(0x21, 0x20);
    CPU::out8(0xA1, 0x28);

    // ICW3, cascade configuration:
    // input does not have a slave
    CPU::out8(0x21, 0x00);
    CPU::out8(0xA1, 0x00);

    // ICW4, environmental configuration:
    // not special fully nested mode | non buffered mode | normal eoi | 8086 mode
    CPU::out8(0x21, 0x01);
    CPU::out8(0xA1, 0x01);

    mask(MASK_ALL, MASTER);
    mask(MASK_ALL, SLAVE);
}

}  // namespace qlib::hardware