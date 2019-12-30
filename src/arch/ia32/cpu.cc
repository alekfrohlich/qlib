#include <arch/ia32/cpu.h>
#include <machine/pc/8259.h>
#include <machine/pc/keyboard.h>
#include <qlib.h>

#include <qlib/ostream.h>

namespace qlib::hardware {

/*________INITIALIZE HARDWARE________________________________________________*/

extern "C" {
void keyboard_handler_main(void) {
    // end of interrupt
    CPU::out8(0x20, 0x20);
    unsigned char status;
    char keycode;

    /* write EOI */
    CPU::out8(0x20, 0x20);

    status = CPU::in8(0x64);  // status port
    /* Lowest bit of status will be set if buffer is not empty */
    if (status & 0x01) {
        keycode = CPU::in8(0x60);  // data port
        if (keycode < 0) return;
        qlib::cout << (char) keyboard_map[(unsigned char) keycode];
    }
}
void keyboard_handler(void);
}

typedef CPU::GDT_Entry GDT_Entry;
typedef CPU::IDT_Entry IDT_Entry;

// @TODO: malloc this!
// create tree entries: NULL, CODE, DATA
static GDT_Entry gdtptr[3] = {
    gdtptr[0] = GDT_Entry(0, 0x00000, 0x0, 0x00),
    gdtptr[1] = GDT_Entry(0, 0xfffff, 0xc, 0x9A),
    gdtptr[2] = GDT_Entry(0, 0xfffff, 0xc, 0x92),
};

// create IDT entry for keyboard interrupts
static IDT_Entry idtptr[256];

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
// @TODO: Get rid of the copying!
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

void CPU::default_init() {
    // load gdtr
    Reg16 size = sizeof(GDT_Entry) * 3 - 1;
    Reg32 ptr = reinterpret_cast<Reg32>(gdtptr);
    gdtr(size, ptr);

    // reload segment registers
    cs(0x08);
    ds(0x10);

    Reg32 keyboard_address = reinterpret_cast<Lin_Address>(keyboard_handler);
    idtptr[0x21] = IDT_Entry(0x08, 0x8e, keyboard_address);

    // load idtr
    size = sizeof(IDT_Entry) * 256 - 1;
    ptr = reinterpret_cast<Reg32>(idtptr);
    // ptr = reinterpret_cast<Reg32>(idtptr);
    idtr(size, ptr);

    PIC::init();

    int_enable();

    PIC::unmask(PIC::KEYBOARD_LINE);
}

}  // namespace qlib::hardware
