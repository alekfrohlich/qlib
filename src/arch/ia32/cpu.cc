#include <arch/cpu.h>
#include <arch/ia32/cpu.h>
#include <std/ostream.h>


struct CPU::IDTEntry CPU::IDT[IDT_ENTRIES];

/*________INITIALIZE HARDWARE________________________________________________*/

static void keyboard_handler() {
    std::cout << "a";
}

void CPU::init() {

    //@TODO: setup GDT

    // setup idt
    Reg32 keyboard_address;
	Reg32 idt_address;
	Reg32 idt_base;
    Reg16 idt_limit;

	/* populate IDT entry of keyboard's interrupt */
	keyboard_address = (unsigned long)keyboard_handler;
	IDT[0x21].offset_lowerbits = keyboard_address & 0xffff;
	IDT[0x21].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[0x21].zero = 0;
	IDT[0x21].type_attr = 0x8e; /* INTERRUPT_GATE */
	IDT[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;

    //@TODO: Move this code to a PIC Class init
	/*     Ports
	*	 PIC1	PIC2
	*Command 0x20	0xA0
	*Data	 0x21	0xA1
	*/
	// ICW1
    out8(0x20 , 0x11);
	out8(0xA0 , 0x11);
	// ICW2
    out8(0x21 , 0x20);
	out8(0xA1 , 0x28);
	// ICW3
    out8(0x21 , 0x00);
	out8(0xA1 , 0x00);
    // ICW4
	out8(0x21 , 0x01);
	out8(0xA1 , 0x01);
    // mask interrupts
	out8(0x21 , 0xff);
	out8(0xA1 , 0xff);

	/* fill the IDT descriptor */
	idt_address = (unsigned long)IDT ;
	idt_base = (sizeof (struct IDTEntry) * IDT_ENTRIES) + ((idt_address & 0xffff) << 16);
	idt_limit = idt_address >> 16 ;

	loadidt(idt_base, idt_limit);

    // unmask interrupts
	out8(0x21 , 0xFD);

    //@TODO: setup TSS

}

/*________SPECIAL REGISTERS__________________________________________________*/

void CPU::loadidt(Reg32 base, Reg16 limit) {
    volatile Reg8 aux[6];
    volatile Reg16 * l = reinterpret_cast<volatile Reg16 *>(&aux[0]);
    volatile Reg32 * b = reinterpret_cast<volatile Reg32 *>(&aux[2]);

    *l = limit;
    *b = base;
    asm("lidt %0" : : "m" (aux[0]));
}


/*________IO PORT INTERFACE__________________________________________________*/

CPU_Common::Reg8 CPU::in8(const IOPort & port) {
    Reg8 value;
    asm("inb %1,%0" : "=a"(value) : "d"(port));
    return value;
}

CPU_Common::Reg16 CPU::in16(const IOPort & port) {
    Reg16 value;
    asm("inw %1,%0" : "=a"(value) : "d"(port));
    return value;
}

CPU_Common::Reg32 CPU::in32(const IOPort & port) {
    Reg32 value;
    asm("inl %1,%0" : "=a"(value) : "d"(port));
    return value;
}

void CPU::out8(const IOPort & port, const CPU_Common::Reg8 & value) {
    asm("outb %1,%0" : : "d"(port), "a"(value));
}

void CPU::out16(const IOPort & port, const CPU_Common::Reg16 & value) {
    asm("outw %1,%0" : : "d"(port), "a"(value));
}

void CPU::out32(const IOPort & port, const CPU_Common::Reg32 & value) {
    asm("outl %1,%0" : : "d"(port), "a"(value));
}