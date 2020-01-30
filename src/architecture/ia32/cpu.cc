#include <architecture/cpu.h>
#include <machine/ic.h>
#include <thread.h>

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
}

void CPU::switch_context(Context & o, Context & n) {
    // save flags register (no need to move into)
    ASM("pushf");

    // save general purpose registers (no need to move into)
    ASM("   mov %%ebx, %0" ::"m"(o.ebx));
    ASM("   mov %%esi, %0" ::"m"(o.esi));
    ASM("   mov %%edi, %0" ::"m"(o.edi));

    // save stack registers
    ASM("   mov 0x0(%%ebp), %%eax   \n"  // no need to move into
        "   mov %%eax,      %0      \n" ::"m"(o.ebp)
        : "eax");
    ASM("   mov %%esp,      %0      \n" ::"m"(o.esp));

    // save instruction pointer (no need to mov into)
    ASM("   mov 0x4(%%ebp), %%eax   \n"
        "   mov %%eax,     %0       \n" ::"m"(o.eip)
        : "eax");

    // restore stack pointer
    ASM("mov %0, %%esp" : "=m"(n.esp) :);

    // new thread? prepare landing stack
    if (n.ebp == 0) {
        // push exit so that the thread is automatically cleaned up after
        // finishing executing
        // ASM("push %0" : : "i"(reinterpret_cast<Log_Address>(Thread::exit)));
        // @TODO: verfify inputs and outputs
        ASM("push %0" : "=m"(n.eip) :);
        ASM("push %0" : "=m"(n.ebp) :);
        ASM("push %0" : "=m"(n.eflags) :);
    }

    // restore general purpose registers
    ASM("   mov %0, %%ebx" : "=m"(n.ebx) :);
    ASM("   mov %0, %%esi" : "=m"(n.esi) :);
    ASM("   mov %0, %%edi" : "=m"(n.edi) :);

    // restore flags
    ASM("popf");

    // calling convention garantees that ebp and eip are restored
}

}  // namespace qlib::mediator
