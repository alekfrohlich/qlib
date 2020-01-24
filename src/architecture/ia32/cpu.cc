#include <architecture/cpu.h>
#include <machine/ic.h>

namespace qlib::mediator {

using GDT_Entry = CPU::GDT_Entry;
using IDT_Entry = CPU::IDT_Entry;

static GDT_Entry gdt[3] = {
    GDT_Entry(0, 0x00000, GDT_Entry::ZERO, GDT_Entry::ZERO),
    GDT_Entry(
        0, 0xfffff, GDT_Entry::PAGE_GR_AND_32BIT_SEL, GDT_Entry::TEXT_SEG),
    GDT_Entry(
        0, 0xfffff, GDT_Entry::PAGE_GR_AND_32BIT_SEL, GDT_Entry::DATA_SEG),
};
static IDT_Entry idt[256];

void CPU::init(void) {
    // load gdtr
    Reg16 size = sizeof(GDT_Entry) * 3 - 1;
    Reg32 ptr = reinterpret_cast<Reg32>(gdt);
    gdtr(size, ptr);
    CPU::gdt_ptr = gdt;

    db<CPU>() << "Global Descriptor Table Entries\n"
              << gdt[0] << "\n"
              << gdt[1] << "\n"
              << gdt[2] << "\n";

    // reload segment registers
    cs(0x08);
    ds(0x10);

    // load idtr
    size = sizeof(IDT_Entry) * 256 - 1;
    ptr = reinterpret_cast<Reg32>(idt);
    idtr(size, ptr);
    CPU::idt_ptr = idt;

    // fill idt
    for (int i = 0; i < 256; i++)
        idt[i] = IDT_Entry(CPU::cs(), IDT_Entry::INTGATE_32, CPU::halt);

    int_enable();
}

}  // namespace qlib::mediator
