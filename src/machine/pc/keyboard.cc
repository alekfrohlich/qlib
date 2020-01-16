#include <arch/cpu.h>
#include <machine/ic.h>
#include <machine/keyboard.h>

namespace qlib::mediator {

[[gnu::interrupt]] static void keyboard_handler(void * ignored) {
    Keyboard::default_handler();
}

void Keyboard::init() {
    CPU::idt_ptr[0x21].isr((unsigned) keyboard_handler);
}

}  // namespace qlib::mediator
