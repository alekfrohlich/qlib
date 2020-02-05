#include <architecture/cpu.h>
#include <machine/ic.h>
#include <system.h>

namespace qlib {

void CPU::init(void) {
    // load gdtr
    gdt = new (SYSTEM) GDT_Entry[3] {
        GDT_Entry(0, 0x00000, GDT_Entry::ZERO, GDT_Entry::ZERO),
        GDT_Entry(
            0, 0xfffff, GDT_Entry::PAGE_GR_AND_32BIT_SEL, GDT_Entry::TEXT_SEG),
        GDT_Entry(
            0, 0xfffff, GDT_Entry::PAGE_GR_AND_32BIT_SEL, GDT_Entry::DATA_SEG),
    };
    Reg16 size = sizeof(GDT_Entry) * 3 - 1;
    Reg32 ptr = reinterpret_cast<Reg32>(gdt);
    gdtr(size, ptr);

    db<CPU>() << "Global Descriptor Table Entries\n"
              << gdt[0] << "\n"
              << gdt[1] << "\n"
              << gdt[2] << "\n";

    // reload segment registers
    cs(0x08);
    ds(0x10);

    // load idtr
    idt = new (SYSTEM) IDT_Entry[256];
    size = sizeof(IDT_Entry) * 256 - 1;
    ptr = reinterpret_cast<Reg32>(idt);
    idtr(size, ptr);

    // fill idt
    for (int i = 0; i < 256; i++)
        idt[i] = IDT_Entry(CPU::cs(), IDT_Entry::INTGATE_32, CPU::halt);
}
};  // namespace qlib
