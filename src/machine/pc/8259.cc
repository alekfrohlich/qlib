#include <arch/ia32/cpu.h>
#include <machine/pc/8259.h>

const CPU::IOPort PIC::PIC1_STATUS = static_cast<CPU::IOPort>(0x20);
const CPU::IOPort PIC::PIC1_DATA = static_cast<CPU::IOPort>(0x21);
const CPU::IOPort PIC::PIC2_STATUS = static_cast<CPU::IOPort>(0xA0);
const CPU::IOPort PIC::PIC2_DATA = static_cast<CPU::IOPort>(0xA1);
PIC::Mask PIC::_mask = static_cast<PIC::Mask>(0xff);

/*________INITIALIZE HARDWARE________________________________________________*/

void PIC::init(void) {
  // ICW1
  CPU::out8(0x20, 0x11);
  CPU::out8(0xA0, 0x11);
  // ICW2
  CPU::out8(0x21, 0x20);
  CPU::out8(0xA1, 0x28);
  // ICW3
  CPU::out8(0x21, 0x00);
  CPU::out8(0xA1, 0x00);
  // ICW4
  CPU::out8(0x21, 0x01);
  CPU::out8(0xA1, 0x01);
  // mask interrupts
  CPU::out8(0x21, 0xff);
  CPU::out8(0xA1, 0xff);
}
