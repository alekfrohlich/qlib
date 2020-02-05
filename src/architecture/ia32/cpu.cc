#include <architecture/cpu.h>

namespace qlib::mediator {
void CPU::Context::load() volatile {
    volatile Context * context = this;
    ASM("mov %0, %%esp" : "=m"(context) :);

    // restore general purpose registers
    ASM("popa");

    // restore flags
    ASM("popf");

    ASM("pop %ebp");

    ASM("ret");
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
    ASM("mov %0, %%esp" : "=m"(to) :);

    // restore general purpose registers
    ASM("popa");

    // restore flags
    ASM("popf");

    // calling convention pops ebp/eip
}
};  // namespace qlib::mediator
