#ifndef _QLIB_HARDWARE_CPU_H
#define _QLIB_HARDWARE_CPU_H

#include <qlib.h>

namespace qlib::mediator {

class CPU
{
 public:
    /*________I386 TYPES_________________________________________________________*/

    using Reg8 = unsigned char;
    using Reg16 = unsigned short;
    using Reg32 = unsigned long;
    using Reg64 = unsigned long long;
    using Log_Address = unsigned long;
    using Lin_Address = unsigned long;
    using Phy_Address = unsigned long;
    using IOPort = unsigned short;

    struct [[gnu::packed]] GDT_Entry {
     public:
        // Pr | Privl | S | Ex | DC | RW | Ac
        enum Access {
            TEXT_SEG = 0x9A,
            DATA_SEG = 0x92,
        };

        // Gr | Sz | 0 | 0
        enum Flags {
            ZERO = 0x0,                  // null descriptor
            PAGE_GR_AND_32BIT_SEL = 0xc  // page granularity and 32 bit selector
        };

        constexpr GDT_Entry(
            unsigned base, unsigned limit, unsigned flags, unsigned access)
            : base_low {base & 0xffffff}, base_high {(base >> 24) & 0xff},
              limit_low {(limit & 0xffff)}, limit_high {(limit >> 16) & 0xf},
              flags {flags}, access {access} {}

     private:
        unsigned limit_low : 16;
        unsigned base_low : 24;
        unsigned access : 8;
        unsigned limit_high : 4;
        unsigned flags : 4;
        unsigned base_high : 8;
    };

    struct [[gnu::packed]] IDT_Entry {
     public:
        enum Types {
            TASKGATE_32 = 0x85,
            INTGATE_16 = 0x86,
            TRAPGATE_16 = 0x87,
            INTGATE_32 = 0x8e,
            TRAPGATE_32 = 0x8f,
        };

        constexpr IDT_Entry() {}
        constexpr IDT_Entry(unsigned selector, unsigned type, void (*isr)())
            : offset_low {(Log_Address) isr & 0xffff}, selector {selector},
              zero {0}, type {type},
              offset_high {((Log_Address) isr & 0xffff0000) >> 16} {}

        void isr(unsigned new_isr) {
            offset_low = (Log_Address) new_isr & 0xffff;
            offset_high = ((Log_Address) new_isr & 0xffff0000) >> 16;
        }

     private:
        unsigned offset_low : 16;
        unsigned selector : 16;
        unsigned zero : 8;
        unsigned type : 8;
        unsigned offset_high : 16;
    };

    inline static GDT_Entry * gdt_ptr = nullptr;
    inline static IDT_Entry * idt_ptr = nullptr;

    //========INITIALIZATION=====================================================//
    // Setup Global Descriptor Table (GDT) and Interrupt Descriptor Table (IDT).
    // Also configures Programmable Interrupt Controller (PIC) to enable irq1,
    // aka the keyboard line.
    //===========================================================================//

    static void init(void);

    /*________MISCELLANEOUS__________________________________________________*/

    // halt is used as a function for filling the idt
    static void halt(void) { ASM("hlt"); }

    /*________ENABLE/DISABLE INTERRUPTS______________________________________*/

    INTRIN void int_enable(void) { ASM("sti"); }

    INTRIN void int_disable(void) { ASM("cli"); }

    /*________DESCRIPTOR TABLE REGISTERS_____________________________________*/

    INTRIN void idtr(Reg16 limit, Reg32 base) {
        struct [[gnu::packed]] {
            unsigned limit : 16;
            unsigned base : 32;
        }
        idtptr = {limit, base};

        ASM("lidt %0" : : "m"(idtptr));
    }

    INTRIN void idtr(Reg16 * limit, Reg32 * base) {
        struct [[gnu::packed]] {
            unsigned limit : 16;
            unsigned base : 32;
        }
        idtr;

        ASM("sidt %0 " : "=m"(idtr) :);
        *limit = idtr.limit;
        *base = idtr.base;
    }

    INTRIN void gdtr(Reg16 size, Reg32 limit) {
        struct [[gnu::packed]] {
            unsigned limit : 16;
            unsigned base : 32;
        }
        gdtr = {size, limit};

        ASM("lgdt %0" : : "m"(gdtr));
    }

    INTRIN void gdtr(Reg16 * limit, Reg32 * base) {
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

    // @OBS: Qlib does not attempt to provide full segmentation support!

    INTRIN void cs(const Reg16 val) {
        ASM("ljmp %0, $1f   \n"
            "1: nop"
            :
            : "i"(val));
    }

    INTRIN Reg16 cs() {
        Reg16 cs;
        ASM("movw %%cs, %0" : "=r"(cs) :);
        return cs;
    }

    INTRIN void ds(Reg16 val) { ASM("movw %0, %%ds" : : "im"(val)); }

    INTRIN Reg16 ds() {
        Reg16 ds;
        ASM("movw %%ds, %0" : "=r"(ds) :);
        return ds;
    }

    INTRIN Reg16 es() {
        Reg16 es;
        ASM("movw %%es, %0" : "=r"(es) :);
        return es;
    }

    INTRIN Reg16 fs() {
        Reg16 value;
        ASM("mov %%cs,%0" : "=r"(value) :);
        return value;
    }

    INTRIN Reg16 gs() {
        Reg16 gs;
        ASM("movw %%gs, %0" : "=r"(gs) :);
        return gs;
    }

    INTRIN Reg16 ss() {
        Reg16 ss;
        ASM("movw %%ss, %0" : "=r"(ss) :);
        return ss;
    }

    /*________IO PORT INTERFACE__________________________________________________*/

    // IN / OUT only accept 8-bit immediates. So optmizing the following could would
    // checking wether port > 0xff

    INTRIN Reg8 in8(IOPort port) {
        Reg8 value;
        ASM("inb %1,%0" : "=a"(value) : "d"(port));
        return value;
    }

    INTRIN Reg16 in16(IOPort port) {
        Reg16 value;
        ASM("inw %1,%0" : "=a"(value) : "d"(port));
        return value;
    }

    INTRIN Reg32 in32(IOPort port) {
        Reg32 value;
        ASM("inl %1,%0" : "=a"(value) : "d"(port));
        return value;
    }

    INTRIN void out8(IOPort port, Reg8 value) {
        ASM("outb %1,%0" : : "d"(port), "a"(value));
    }

    // currently bugged in gcc10
    INTRIN void out16(IOPort port, Reg16 value) {
        ASM("outw %1,%0" : : "d"(port), "a"(value));
    }

    INTRIN void out32(IOPort port, Reg32 value) {
        ASM("outl %1,%0" : : "d"(port), "a"(value));
    }
};

}  // namespace qlib::mediator

#endif  // _QLIB_HARDWARE_CPU_H
