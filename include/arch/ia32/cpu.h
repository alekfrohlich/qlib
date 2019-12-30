#ifndef _QLIB_HARDWARE_CPU_H
#define _QLIB_HARDWARE_CPU_H

#include <qlib.h>

namespace qlib::hardware {

class CPU
{
 public:
    /*________I386 TYPES_________________________________________________________*/

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
    // @TODO: Check if integer types are really architecture independent.
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    typedef unsigned long Reg;
    typedef unsigned char Reg8;
    typedef unsigned short Reg16;
    typedef unsigned long Reg32;
    typedef unsigned long long Reg64;
    typedef unsigned long Log_Address;
    typedef unsigned long Lin_Address;
    typedef unsigned short IOPort;

    struct [[gnu::packed]] GDT_Entry {
     public:
        GDT_Entry(unsigned base, unsigned limit, unsigned granularity,
            unsigned access)
            : base_low {base & 0xffffff}, base_high {(base >> 24) & 0xff},
              limit_low {(limit & 0xffff)}, limit_high {(limit >> 16) & 0xf},
              granularity {granularity}, access {access} {}

     private:
        unsigned limit_low : 16;
        unsigned base_low : 24;
        unsigned access : 8;
        unsigned limit_high : 4;
        unsigned granularity : 4;
        unsigned base_high : 8;
    };

    struct [[gnu::packed]] IDT_Entry {
     public:
        IDT_Entry() {}
        IDT_Entry(unsigned selector, unsigned type, unsigned isr)
            : offset_low {isr & 0xffff}, selector {selector}, zero {0},
              type {type}, offset_high {(isr & 0xffff0000) >> 16} {}

     private:
        unsigned offset_low : 16;
        unsigned selector : 16;
        unsigned zero : 8;
        unsigned type : 8;
        unsigned offset_high : 16;
    };

    //========DEFAULT INIT=======================================================//
    // setup Global Descriptor Table (GDT) and Interrupt Descriptor Table (IDT)
    // also configures Programmable Interrupt Controller (PIC) to enable irq1,
    // aka the keyboard line.
    //===========================================================================//

    static void default_init(void);

    /*________ENABLE/DISABLE INTERRUPTS______________________________________*/

    INSTRINSIC void int_enable(void) { ASM("sti"); }

    INSTRINSIC void int_disable(void) { ASM("cli"); }

    /*________DESCRIPTOR TABLE REGISTERS_____________________________________*/

    INSTRINSIC void idtr(Reg16 limit, Reg32 base) {
        struct [[gnu::packed]] {
            unsigned limit : 16;
            unsigned base : 32;
        }
        idtptr = {limit, base};

        ASM("lidt %0" : : "m"(idtptr));
    }

    INSTRINSIC void idtr(Reg16 * limit, Reg32 * base) {
        struct [[gnu::packed]] {
            unsigned limit : 16;
            unsigned base : 32;
        }
        idtr;

        ASM("sidt %0 " : "=m"(idtr) :);
        *limit = idtr.limit;
        *base = idtr.base;
    }

    INSTRINSIC void gdtr(Reg16 size, Reg32 limit) {
        struct [[gnu::packed]] {
            unsigned limit : 16;
            unsigned base : 32;
        }
        gdtr = {size, limit};

        ASM("lgdt %0" : : "m"(gdtr));
    }

    INSTRINSIC void gdtr(Reg16 * limit, Reg32 * base) {
        struct [[gnu::packed]] {
            unsigned limit : 16;
            unsigned base : 32;
        }
        gdtr;

        ASM("sgdt %0 " : "=m"(gdtr) :);
        *limit = gdtr.limit;
        *base = gdtr.base;
    }

    /*________SEGMENT REGISTERS__________________________________________________*/

    INSTRINSIC void cs(const Reg16 val) {
        ASM("ljmp %0, $1f   \n"
            "1: nop"
            :
            : "n"(val));
    }

    INSTRINSIC Reg16 cs() {
        volatile Reg16 cs;
        ASM("movw %%cs, %0" : : "m"(cs));
        return cs;
    }

    INSTRINSIC void ds(Reg16 val) { ASM("movw %0, %%ds" : : "m"(val)); }

    INSTRINSIC Reg16 ds() {
        volatile Reg16 ds;
        ASM("movw %%ds, %0" : "=m"(ds) :);
        return ds;
    }

    INSTRINSIC void es(Reg16 val) { ASM("movw %0, %%es" : : "m"(val)); }

    INSTRINSIC Reg16 es() {
        volatile Reg16 es;
        ASM("movw %%es, %0" : "=m"(es) :);
        return es;
    }

    INSTRINSIC void fs(Reg16 val) { ASM("movw %0, %%fs" : : "m"(val)); }

    INSTRINSIC Reg16 fs() {
        volatile Reg16 fs;
        ASM("movw %%fs, %0" : "=m"(fs) :);
        return fs;
    }

    INSTRINSIC void gs(Reg16 val) { ASM("movw %0, %%gs" : : "m"(val)); }

    INSTRINSIC Reg16 gs() {
        volatile Reg16 gs;
        ASM("movw %%gs, %0" : "=m"(gs) :);
        return gs;
    }

    INSTRINSIC void ss(Reg16 val) { ASM("movw %0, %%ss" : : "m"(val)); }

    INSTRINSIC Reg16 ss() {
        volatile Reg16 ss;
        ASM("movw %%ss, %0" : "=m"(ss) :);
        return ss;
    }

    /*________IO PORT INTERFACE__________________________________________________*/

    INSTRINSIC Reg8 in8(IOPort port) {
        Reg8 value;
        ASM("inb %1,%0" : "=a"(value) : "d"(port));
        return value;
    }

    INSTRINSIC Reg16 in16(IOPort port) {
        Reg16 value;
        ASM("inw %1,%0" : "=a"(value) : "d"(port));
        return value;
    }

    INSTRINSIC Reg32 in32(IOPort port) {
        Reg32 value;
        ASM("inl %1,%0" : "=a"(value) : "d"(port));
        return value;
    }

    INSTRINSIC void out8(IOPort port, Reg8 value) {
        ASM("outb %1,%0" : : "d"(port), "a"(value));
    }

    INSTRINSIC void out16(IOPort port, Reg16 value) {
        ASM("outw %1,%0" : : "d"(port), "a"(value));
    }

    static void out32(const IOPort & port, const Reg32 & value) {
        ASM("outl %1,%0" : : "d"(port), "a"(value));
    }
};

}  // namespace qlib::hardware

#endif  // _QLIB_HARDWARE_CPU_H
