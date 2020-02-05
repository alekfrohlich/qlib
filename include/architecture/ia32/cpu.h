#ifndef __QLIB_MEDIATOR_PC_CPU_H
#define __QLIB_MEDIATOR_PC_CPU_H

#include <architecture/cpu.h>
// @TODO: how to provide new for everyone?
#include <system.h>

namespace qlib::mediator {

class CPU
{
 public:
    using Reg8 = unsigned char;
    using Reg16 = unsigned short;
    using Reg32 = unsigned long;
    using Reg64 = unsigned long long;
    using Log_Address = unsigned long;
    using Lin_Address = unsigned long;
    using Phy_Address = unsigned long;
    using IO_Port = unsigned short;

    // [15:0]  CF | 1 | PF | | AF | | ZF | SF | TF | IF | DF | OF | IOPL | NT | |
    // [31:16] RF | VM | AC | VIF | VIP | ID | ... |
    enum EFlags {
        DEFAULT = 1 << 1,
    };

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

        friend Debug & operator<<(Debug & db, const GDT_Entry & gdte) {
            db << "limit_low = " << gdte.limit_low << "\n"
               << "base_low = " << gdte.base_low << "\n"
               << "access = " << gdte.access << "\n"
               << "limit_high = " << gdte.limit_high << "\n"
               << "flags = " << gdte.flags << "\n"
               << "base_high = " << gdte.base_high << "\n";
            return db;
        }

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

        friend Debug & operator<<(Debug & db, const IDT_Entry & idte) {
            db << "offset_low = " << idte.offset_low << "\n"
               << "selector = " << idte.selector << "\n"
               << "zero = " << idte.zero << "\n"
               << "type = " << idte.type << "\n"
               << "offset_high = " << idte.offset_high << "\n";
            return db;
        }

     private:
        unsigned offset_low : 16;
        unsigned selector : 16;
        unsigned zero : 8;
        unsigned type : 8;
        unsigned offset_high : 16;
    };

    struct [[gnu::packed]] Context {
        Context(Log_Address entry) : eflags(EFlags::DEFAULT), eip(entry) {}

        void load() volatile;

        friend Debug & operator<<(Debug & db, const Context & c) {
            db << "esp (=this) = " << &c << "\n"
               << "edi = " << c.edi << "\n"
               << "esi = " << c.esi << "\n"
               << "ebx = " << c.ebx << "\n"
               << "edx = " << c.edx << "\n"
               << "ecx = " << c.ecx << "\n"
               << "eax = " << c.eax << "\n"
               << "eflags = " << c.eflags << "\n"
               << "ebp = " << c.ebp << "\n"
               << "eip = " << c.eip << "\n";
            return db;
        }

        Reg32 edi;
        Reg32 esi;
        Reg32 _ebp;  // redundant (ebp is already saved by calling convention)
        Reg32 _esp;  // redundant (esp == this)
        Reg32 ebx;
        Reg32 edx;
        Reg32 ecx;
        Reg32 eax;
        Reg32 eflags;
        Reg32 ebp;
        Reg32 eip;
    };

    inline static GDT_Entry * gdt_ptr = nullptr;
    inline static IDT_Entry * idt_ptr = nullptr;

    // Setup Global Descriptor Table (GDT) and Interrupt Descriptor Table (IDT).
    static void init();

    // @TODO: allow entry to have any user-defined params
    static Context * init_stack(char * stack, void (*exit)(), int (*entry)()) {
        // push exit so that the thread is automatically cleaned up
        // after it finishes executing
        stack -= sizeof(Log_Address);
        new (stack) Log_Address(reinterpret_cast<Log_Address>(exit));

        stack -= sizeof(Context);
        return new (stack) Context(reinterpret_cast<Log_Address>(entry));
    }

    static void switch_context(
        Context * volatile * from, volatile Context * to);

    static void halt() { ASM("hlt"); }

    static void int_enable() { ASM("sti"); }

    static void int_disable() { ASM("cli"); }

    static Reg32 eflags() {
        Reg32 value;
        ASM("pushfl");
        ASM("popl %0" : "=r"(value) :);
        return value;
    }

    static void eflags(Reg32 value) {
        ASM("pushl %0" : : "r"(value));
        ASM("popfl");
    }

    static void idtr(Reg16 limit, Reg32 base) {
        struct [[gnu::packed]] {
            unsigned limit : 16;
            unsigned base : 32;
        }
        idtptr = {limit, base};

        ASM("lidt %0" : : "m"(idtptr));
    }

    static void idtr(Reg16 * limit, Reg32 * base) {
        struct [[gnu::packed]] {
            unsigned limit : 16;
            unsigned base : 32;
        }
        idtr;

        ASM("sidt %0 " : "=m"(idtr) :);
        *limit = idtr.limit;
        *base = idtr.base;
    }

    static void gdtr(Reg16 size, Reg32 limit) {
        struct [[gnu::packed]] {
            unsigned limit : 16;
            unsigned base : 32;
        }
        gdtr = {size, limit};

        ASM("lgdt %0" : : "m"(gdtr));
    }

    static void gdtr(Reg16 * limit, Reg32 * base) {
        struct [[gnu::packed]] {
            unsigned limit : 16;
            unsigned base : 32;
        }
        gdtr;

        ASM("sgdt %0 " : "=m"(gdtr) :);
        *limit = gdtr.limit;
        *base = gdtr.base;
    }

    [[gnu::always_inline, gnu::artificial]] static inline void cs(
        const Reg16 val) {
        ASM("ljmp %0, $1f   \n"
            "1: nop"
            :
            : "i"(val));
    }

    static Reg16 cs() {
        Reg16 cs;
        ASM("movw %%cs, %0" : "=r"(cs) :);
        return cs;
    }

    static void ds(Reg16 val) { ASM("movw %0, %%ds" : : "im"(val)); }

    static Reg16 ds() {
        Reg16 ds;
        ASM("movw %%ds, %0" : "=r"(ds) :);
        return ds;
    }

    static Reg16 es() {
        Reg16 es;
        ASM("movw %%es, %0" : "=r"(es) :);
        return es;
    }

    static Reg16 fs() {
        Reg16 value;
        ASM("mov %%cs,%0" : "=r"(value) :);
        return value;
    }

    static Reg16 gs() {
        Reg16 gs;
        ASM("movw %%gs, %0" : "=r"(gs) :);
        return gs;
    }

    static Reg16 ss() {
        Reg16 ss;
        ASM("movw %%ss, %0" : "=r"(ss) :);
        return ss;
    }

    static Reg8 in8(IO_Port port) {
        Reg8 value;
        ASM("inb %1,%0" : "=a"(value) : "d"(port));
        return value;
    }

    static Reg16 in16(IO_Port port) {
        Reg16 value;
        ASM("inw %1,%0" : "=a"(value) : "d"(port));
        return value;
    }

    static Reg32 in32(IO_Port port) {
        Reg32 value;
        ASM("inl %1,%0" : "=a"(value) : "d"(port));
        return value;
    }

    static void out8(IO_Port port, Reg8 value) {
        ASM("outb %1,%0" : : "d"(port), "a"(value));
    }

    static void out16(IO_Port port, Reg16 value) {
        ASM("outw %1,%0" : : "d"(port), "a"(value));
    }

    static void out32(IO_Port port, Reg32 value) {
        ASM("outl %1,%0" : : "d"(port), "a"(value));
    }
};

}  // namespace qlib::mediator

#endif  // __QLIB_MEDIATOR_PC_CPU_H
