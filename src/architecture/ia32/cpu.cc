#include <architecture/cpu.h>

namespace qlib {
void __attribute__((noinline)) CPU::Context::load() volatile {
    ASM("mov %0, %%esp" : : "g"(this));

    // restore general purpose registers
    ASM("popa");

    // restore flags
    ASM("popf");

    // calling convention pops ebp/eip
}

// this code depends on the function returning and popping the stack frame
// so it cannot be inlined
void __attribute__((noinline))
CPU::switch_context(Context * volatile * from, volatile Context * to) {
    // calling convention pushes eip/ebp

    // save flags register
    ASM("pushf");

    // save general purpose registers
    ASM("pusha");

    // adjust from's context
    ASM("   mov %0,    %%eax    \n"
        "   mov %%esp,(%%eax)   \n" ::"m"(from)
        : "eax");

    // restore stack pointer
    ASM("mov %0, %%esp" : : "m"(to));

    // restore general purpose registers
    ASM("popa");

    // restore flags
    ASM("popf");

    // calling convention pops ebp/eip
}
};  // namespace qlib
