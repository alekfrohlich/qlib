#ifndef CPU_H
#define CPU_H

#include <arch/cpu.h>

//@TODO: private inheritance
class CPU : CPU_Common
{
 public:

    typedef Reg16 IOPort;

    // idt entries are called gates
    struct IDTEntry {
        Reg16 offset_lowerbits;
        Reg16 selector;
        Reg8 zero;
        Reg8 type_attr;
        Reg16 offset_higherbits;
    };

    static const unsigned int IDT_ENTRIES = 256;
    static struct IDTEntry IDT[IDT_ENTRIES];

    static void init(void);

    //@: why not const?
    static void loadidt(Reg32 base, Reg16 limit);

    static Reg8 in8(const IOPort & port);
    static Reg16 in16(const IOPort & port);
    static Reg32 in32(const IOPort & port);

    static void out8(const IOPort & port, const Reg8 & value);
    static void out16(const IOPort & port, const Reg16 & value);
    static void out32(const IOPort & port, const Reg32 & value);
};

#endif // CPU_H
