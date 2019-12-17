#ifndef CPU_H
#define CPU_H

#include <arch/cpu.h>
#include <qlib.h>

namespace qlib::hardware {

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

    struct IDT_Entry {
        unsigned offset_low : 16;
        unsigned selector : 16;
        unsigned zero : 8;
        unsigned type : 8;
        unsigned offset_high : 16;
    } __attribute__((packed));

    static const unsigned IDT_ENTRIES = 256;
    static IDT_Entry IDT[IDT_ENTRIES];
    static const unsigned GDT_ENTRIES = 3;
    static GDT_Entry GDT[GDT_ENTRIES];

    // setup intel quirks
    static void init(void);

    /*________ENABLE/DISABLE INTS________________________________________________*/

    static void int_enable(void) { ASM("sti"); }

    static void int_disable(void) { ASM("cli"); }

    /*________SPECIAL REGISTERS__________________________________________________*/

    static void load_idt(Reg16 size, Reg32 ptr) {
        struct {
            unsigned size : 16;
            unsigned ptr : 32;
        } __attribute__((packed)) idtptr = {size, ptr};

        ASM("lidt %0" : : "m"(idtptr));
    }

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
    // @TODO: segregate reloading registers from load_gdt
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    static void load_gdt(Reg16 size, Reg32 ptr) {
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

    /*________SEGMENT REGISTERS__________________________________________________*/

    // static void cs(const Reg16 & val) {
    //     ASM()
    // }

    /*________IO PORT INTERFACE__________________________________________________*/

    static Reg8 in8(const IOPort & port) {
        Reg8 value;
        ASM("inb %1,%0" : "=a"(value) : "d"(port));
        return value;
    }

    static Reg16 in16(const IOPort & port) {
        Reg16 value;
        ASM("inw %1,%0" : "=a"(value) : "d"(port));
        return value;
    }

    static Reg32 in32(const IOPort & port) {
        Reg32 value;
        ASM("inl %1,%0" : "=a"(value) : "d"(port));
        return value;
    }

    static void out8(const IOPort & port, const Reg8 & value) {
        ASM("outb %1,%0" : : "d"(port), "a"(value));
    }

    static void out16(const IOPort & port, const Reg16 & value) {
        ASM("outw %1,%0" : : "d"(port), "a"(value));
    }

    static void out32(const IOPort & port, const Reg32 & value) {
        ASM("outl %1,%0" : : "d"(port), "a"(value));
    }
};

}  // namespace qlib::hardware

#endif  // CPU_H
