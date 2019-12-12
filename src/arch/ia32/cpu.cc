#include <arch/cpu.h>
#include <arch/ia32/cpu.h>

#include <std/ostream.h>

#define ASM __asm__ __volatile__
CPU::IDTEntry CPU::IDT[IDT_ENTRIES];
CPU::gdte CPU::GDT[GDT_ENTRIES];

/*________INITIALIZE HARDWARE________________________________________________*/

extern "C" {
void keyboard_handler_main(void) {
    // end of interrupt
    CPU::out8(0x20, 0x20);
    std::cout << "interrompido\n";
}
extern void keyboard_handler(void);
extern void gdt_flush(CPU::gdtptr * gdtptr);
}

static void set_gdte(int n, unsigned base, unsigned limit, unsigned granularity,
    unsigned access) {
    CPU::GDT[n].base_low = (base & 0xffffff);
    CPU::GDT[n].base_high = (base >> 24) & 0xff;
    CPU::GDT[n].limit_low = (limit & 0xffff);
    CPU::GDT[n].limit_high = (limit >> 16) & 0xf;
    CPU::GDT[n].granularity = granularity;
    CPU::GDT[n].access = access;
}
CPU::gdtptr * gdt_ptr;

void CPU::init() {
    // setup gdt

    // create tree entries: NULL, CODE, DATA
    set_gdte(0, 0, 0x00000, 0x0, 0x00);
    set_gdte(1, 0, 0xfffff, 0xc, 0x9A);
    set_gdte(2, 0, 0xfffff, 0xc, 0x92);

    // fill gdtr
    gdt_ptr->size = sizeof(GDT) - 1;
    gdt_ptr->ptr = (unsigned) GDT;

    // load gdt
    gdt_flush(gdt_ptr);
    while (true)
        ;

    //

    // setup idt
    Reg32 keyboard_address;
    Reg32 idt_address;
    Reg32 idt_ptr[2];

    // create IDT entry for keyboard interrupts
    keyboard_address = reinterpret_cast<LogicalAddr>(keyboard_handler);
    IDT[0x21].offset_lowerbits = keyboard_address & 0xffff;
    IDT[0x21].selector = 0x08;
    IDT[0x21].zero = 0;
    IDT[0x21].type_attr = 0x8e;
    IDT[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;

    // fill idtr
    idt_address = reinterpret_cast<Reg32>(IDT);
    idt_ptr[0] = (sizeof(struct IDTEntry) * IDT_ENTRIES) +
                 ((idt_address & 0xffff) << 16);
    idt_ptr[1] = idt_address >> 16;

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    // @TODO: Move this code to a Programmable Interrupt Controller class (PIC)
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    /*     Ports
	 *	       PIC1	PIC2
	 * Command 0x20	0xA0
	 * Data	   0x21	0xA1
	 */

    // ICW1
    out8(0x20, 0x11);
    out8(0xA0, 0x11);
    // ICW2
    out8(0x21, 0x20);
    out8(0xA1, 0x28);
    // ICW3
    out8(0x21, 0x00);
    out8(0xA1, 0x00);
    // ICW4
    out8(0x21, 0x01);
    out8(0xA1, 0x01);
    // mask interrupts
    out8(0x21, 0xff);
    out8(0xA1, 0xff);

    // load idt
    load_idt(idt_ptr);

    // enable interrupts
    int_enable();

    // unmask interrupts
    out8(0x21, 0xFD);
}

/*________ENABLE/DISABLE INTS________________________________________________*/

void CPU::int_enable(void) {
    ASM("sti");
}

/*________SPECIAL REGISTERS__________________________________________________*/

void CPU::load_idt(Reg32 * idtptr) {
    ASM("lidt %0" : : "m"(*idtptr));
}

void CPU::load_gdt(Reg32 * gdtptr) {
    ASM("lidt %0" : : "m"(*gdtptr));
}

/*________IO PORT INTERFACE__________________________________________________*/

CPU_Common::Reg8 CPU::in8(const IOPort & port) {
    Reg8 value;
    ASM("inb %1,%0" : "=a"(value) : "d"(port));
    return value;
}

CPU_Common::Reg16 CPU::in16(const IOPort & port) {
    Reg16 value;
    ASM("inw %1,%0" : "=a"(value) : "d"(port));
    return value;
}

CPU_Common::Reg32 CPU::in32(const IOPort & port) {
    Reg32 value;
    ASM("inl %1,%0" : "=a"(value) : "d"(port));
    return value;
}

void CPU::out8(const IOPort & port, const Reg8 & value) {
    ASM("outb %1,%0" : : "d"(port), "a"(value));
}

void CPU::out16(const IOPort & port, const Reg16 & value) {
    ASM("outw %1,%0" : : "d"(port), "a"(value));
}

void CPU::out32(const IOPort & port, const Reg32 & value) {
    ASM("outl %1,%0" : : "d"(port), "a"(value));
}