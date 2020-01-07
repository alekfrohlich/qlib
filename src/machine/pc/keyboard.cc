#include <arch/ia32/cpu.h>
#include <machine/pc/8259.h>
#include <machine/pc/keyboard.h>

namespace qlib::hardware {

__attribute__((interrupt)) static void keyboard_handler(void * ignored) {
    Keyboard::default_handler();
}

void Keyboard::default_init() {
    CPU::idt_ptr[0x21].isr((unsigned) keyboard_handler);
}

}  // namespace qlib::hardware
