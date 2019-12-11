#ifndef CPU_H
#define CPU_H

#include <arch/cpu.h>

//@TODO: private inheritance
class CPU : CPU_Common
{
 public:
    typedef Reg16 IOPort;

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
    // @TODO: add GDT and TSS
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    struct IDTEntry {
        Reg16 offset_lowerbits;
        Reg16 selector;
        Reg8 zero;
        Reg8 type_attr;
        Reg16 offset_higherbits;
    };

    // setup intel quirks
    static void init(void);

    static void int_enable(void);
    static void int_disable(void);

    static void load_idt(Reg32 * idtptr);
    static void load_gdt(Reg32 * gdtptr);
    static void load_tss(Reg32 * tssptr);

    // I/O ports
    static Reg8 in8(const IOPort & port);
    static Reg16 in16(const IOPort & port);
    static Reg32 in32(const IOPort & port);
    static void out8(const IOPort & port, const Reg8 & value);
    static void out16(const IOPort & port, const Reg16 & value);
    static void out32(const IOPort & port, const Reg32 & value);

    static const unsigned int IDT_ENTRIES = 256;
    static IDTEntry IDT[IDT_ENTRIES];
};

#endif  // CPU_H
