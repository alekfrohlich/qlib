#include <arch/cpu.h>
#include <arch/ia32/cpu.h>
#include <machine/pc/8259.h>

#include <std/ostream.h>

#define ASM __asm__ __volatile__
CPU::IDTEntry CPU::IDT[IDT_ENTRIES];
CPU::gdte CPU::GDT[GDT_ENTRIES];

/*________INITIALIZE HARDWARE________________________________________________*/

unsigned char keyboard_map[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8',    /* 9 */
    '9', '0', '-', '=', '\b',                         /* Backspace */
    '\t',                                             /* Tab */
    'q', 'w', 'e', 'r',                               /* 19 */
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',     /* Enter key */
    0,                                                /* 29   - Control */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', /* 39 */
    '\'', '`', 0,                                     /* Left shift */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',               /* 49 */
    'm', ',', '.', '/', 0,                            /* Right shift */
    '*', 0,                                           /* Alt */
    ' ',                                              /* Space bar */
    0,                                                /* Caps lock */
    0,                                                /* 59 - F1 key ... > */
    0, 0, 0, 0, 0, 0, 0, 0, 0,                        /* < ... F10 */
    0,                                                /* 69 - Num lock*/
    0,                                                /* Scroll Lock */
    0,                                                /* Home key */
    0,                                                /* Up Arrow */
    0,                                                /* Page Up */
    '-', 0,                                           /* Left Arrow */
    0, 0,                                             /* Right Arrow */
    '+', 0,                                           /* 79 - End key*/
    0,                                                /* Down Arrow */
    0,                                                /* Page Down */
    0,                                                /* Insert Key */
    0,                                                /* Delete Key */
    0, 0, 0, 0,                                       /* F11 Key */
    0,                                                /* F12 Key */
    0, /* All other keys are undefined */
};

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
        std::cout << (char) keyboard_map[(unsigned char) keycode];
    }
}
extern void keyboard_handler(void);
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

void CPU::init() {
    // setup gdt

    // create tree entries: NULL, CODE, DATA
    set_gdte(0, 0, 0x00000, 0x0, 0x00);
    set_gdte(1, 0, 0xfffff, 0xc, 0x9A);
    set_gdte(2, 0, 0xfffff, 0xc, 0x92);

    Reg16 size = sizeof(GDT) - 1;
    Reg32 ptr = reinterpret_cast<Reg32>(GDT);

    // load gdt and reload segment registers
    load_gdt(size, ptr);

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

    // initialize interrupt controller
    PIC::init();

    // load idt
    load_idt(idt_ptr);

    // enable interrupts
    int_enable();

    // unmask interrupts
    PIC::unmask(PIC::KEYBOARD_LINE);
}

/*________ENABLE/DISABLE INTS________________________________________________*/

void CPU::int_enable(void) {
    ASM("sti");
}

void CPU::int_disable(void) {
    ASM("cli");
}

/*________SPECIAL REGISTERS__________________________________________________*/

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
// @TODO: receive size and ptr as parameters
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

void CPU::load_idt(Reg32 * idtptr) {
    ASM("lidt %0" : : "m"(*idtptr));
}

void CPU::load_gdt(Reg16 size, Reg32 ptr) {
    struct {
        unsigned size : 16;
        unsigned ptr : 32;
    } __attribute__((packed)) gdtptr = {size, ptr};

    ASM("           lgdt %0                                             \n"
        "           ljmp $0x8, $1f                                      \n"
        "           1: movw $0x10, %%ax                                 \n"
        "           movw %%ax, %%ds                                     \n"
        :
        : "m"(gdtptr));
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