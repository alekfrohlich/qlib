#include <architecture/cpu.h>
#include <machine/ic.h>

namespace qlib {

void PIC::init(void) {
    // ICW1
    CPU::out8(MASTER_CMD, ICW1::ICW4_NEEDED | ICW1::ALWAYS);
    CPU::out8(SLAVE_CMD, ICW1::ICW4_NEEDED | ICW1::ALWAYS);

    // ICW2
    CPU::out8(MASTER_DATA, ICW2::FIRST_AVAILABLE);
    CPU::out8(SLAVE_DATA, ICW2::FIRST_AVAILABLE + 0x8);

    // ICW3
    CPU::out8(MASTER_DATA, ICW3::WITHOUT_SLAVES);
    CPU::out8(SLAVE_DATA, ICW3::WITHOUT_SLAVES);

    // ICW4
    CPU::out8(MASTER_DATA, ICW4::MODE_8086);
    CPU::out8(SLAVE_DATA, ICW4::MODE_8086);

    // mask all interrupts
    disable();

    db<IC>() << "Interrupt Mask Register = " << PIC::imr() << "\n"
             << "Interrupt Request Register = " << PIC::irr() << "\n"
             << "Interrupt Status Register = " << PIC::isr() << "\n";
}

}  // namespace qlib