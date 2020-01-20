#include <arch/cpu.h>
#include <machine/ic.h>

namespace qlib::mediator {

/*________INITIALIZE HARDWARE________________________________________________*/

void PIC::init(void) {
  // ICW1
  CPU::out8(PIC1_CMD, ICW1::ICW4_NEEDED | ICW1::ALWAYS);
  CPU::out8(PIC2_CMD, ICW1::ICW4_NEEDED | ICW1::ALWAYS);

  // ICW2
  CPU::out8(PIC1_DATA, ICW2::FIRST_AVAILABLE_PIC1);
  CPU::out8(PIC2_DATA, ICW2::FIRST_AVAILABLE_PIC2);

  // ICW3
  CPU::out8(PIC1_DATA, ICW3::WITHOUT_SLAVES);
  CPU::out8(PIC2_DATA, ICW3::WITHOUT_SLAVES);

  // ICW4
  CPU::out8(PIC1_DATA, ICW4::MODE_8086);
  CPU::out8(PIC2_DATA, ICW4::MODE_8086);

  mask(MASK_ALL, Mode::MASTER);
  mask(MASK_ALL, Mode::SLAVE);

  db<IC>() << "Interrupt Mask Register = " << PIC::imr() << "\n"
           << "Interrupt Request Register = " << PIC::irr() << "\n"
           << "Interrupt Status Register = " << PIC::isr() << "\n";
}

} // namespace qlib::mediator