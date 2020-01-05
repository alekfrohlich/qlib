#include <arch/ia32/cpu.h>
#include <machine/pc/8259.h>
#include <machine/pc/keyboard.h>
#include <qlib.h>

#include <qlib/ostream.h>

namespace qlib::hardware {

/*________INITIALIZE HARDWARE________________________________________________*/

extern "C" {
void keyboard_handler_main(void) {
    unsigned char status;
    char keycode;

    PIC::eoi(PIC::KEYBOARD_LINE);

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
static GDT_Entry gdt[3] = {
    GDT_Entry(0, 0x00000, GDT_Entry::ZERO, GDT_Entry::ZERO),
    GDT_Entry(
        0, 0xfffff, GDT_Entry::PAGE_GR_AND_32BIT_SEL, GDT_Entry::TEXT_SEG),
    GDT_Entry(
        0, 0xfffff, GDT_Entry::PAGE_GR_AND_32BIT_SEL, GDT_Entry::DATA_SEG),
};

// create IDT entry for keyboard interrupts
static IDT_Entry idt[256];

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
// @TODO: Get rid of the copying!
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

static void panic(void) {
    CPU::halt();
}

static void fill_idt(void) {
    for (int i = 0; i < 256; i++)
        idt[i] = IDT_Entry(CPU::cs(), IDT_Entry::INTGATE_32, panic);

    idt[0x21].isr(keyboard_handler);
}

void CPU::default_init() {
    // load gdtr
    Reg16 size = sizeof(GDT_Entry) * 3 - 1;
    Reg32 ptr = reinterpret_cast<Reg32>(gdt);
    gdtr(size, ptr);

    // reload segment registers
    cs(0x08);
    ds(0x10);

    // load idtr
    size = sizeof(IDT_Entry) * 256 - 1;
    ptr = reinterpret_cast<Reg32>(idt);
    idtr(size, ptr);

    fill_idt();
    int_enable();
}

}  // namespace qlib::hardware
