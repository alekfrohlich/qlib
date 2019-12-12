#ifndef CPU_H
#define CPU_H

#include <arch/cpu.h>

//@TODO: private inheritance
class CPU : CPU_Common
{
 public:
    typedef Reg16 IOPort;

    struct gdte {
        unsigned limit_low : 16;  /* Limit low.   */
        unsigned base_low : 24;   /* Base low.    */
        unsigned access : 8;      /* Access.      */
        unsigned limit_high : 4;  /* Limit high.  */
        unsigned granularity : 4; /* Granularity. */
        unsigned base_high : 8;   /* Base high.   */
    } __attribute__((packed));

    struct gdtptr {
        unsigned size : 16; /* GDT size.            */
        unsigned ptr : 32;  /* GDT virtual address. */
    } __attribute__((packed));

    struct IDTEntry {
        unsigned short offset_lowerbits;
        unsigned short selector;
        unsigned char zero;
        unsigned char type_attr;
        unsigned short offset_higherbits;
    } __attribute__((packed));

    // setup intel quirks
    static void init(void);

    static void int_enable(void);
    static void int_disable(void);

    static void load_idt(Reg32 * idtptr);
    static void load_gdt(Reg32 * gdtptr);

    // I/O ports
    static Reg8 in8(const IOPort & port);
    static Reg16 in16(const IOPort & port);
    static Reg32 in32(const IOPort & port);
    static void out8(const IOPort & port, const Reg8 & value);
    static void out16(const IOPort & port, const Reg16 & value);
    static void out32(const IOPort & port, const Reg32 & value);

    static const unsigned IDT_ENTRIES = 256;
    static IDTEntry IDT[IDT_ENTRIES];
    static const unsigned GDT_ENTRIES = 3;
    static gdte GDT[GDT_ENTRIES];
};

#endif  // CPU_H
