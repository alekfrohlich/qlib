#ifndef CPU_H
#define CPU_H

#include <arch/cpu.h>

//@TODO: private inheritance
class CPU : CPU_Common
{
 public:
    typedef Reg16 IOPort;

    struct GDT_Entry {
        unsigned limit_low : 16;
        unsigned base_low : 24;
        unsigned access : 8;
        unsigned limit_high : 4;
        unsigned granularity : 4;
        unsigned base_high : 8;
    } __attribute__((packed));

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
    // @TODO: standarize descriptors
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    struct IDT_Entry {
        unsigned short offset_low;
        unsigned short selector;
        unsigned char zero;
        unsigned char type;
        unsigned short offset_high;
    } __attribute__((packed));

    static const unsigned IDT_ENTRIES = 256;
    static IDT_Entry IDT[IDT_ENTRIES];
    static const unsigned GDT_ENTRIES = 3;
    static GDT_Entry GDT[GDT_ENTRIES];

    // setup intel quirks
    static void init(void);

    static void int_enable(void);
    static void int_disable(void);

    static void load_idt(Reg16 size, Reg32 ptr);
    static void load_gdt(Reg16 size, Reg32 ptr);

    // I/O ports
    static Reg8 in8(const IOPort & port);
    static Reg16 in16(const IOPort & port);
    static Reg32 in32(const IOPort & port);
    static void out8(const IOPort & port, const Reg8 & value);
    static void out16(const IOPort & port, const Reg16 & value);
    static void out32(const IOPort & port, const Reg32 & value);
};

#endif  // CPU_H
