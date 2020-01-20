#ifndef _QLIB_MEDIATOR_PC_IC_H
#define _QLIB_MEDIATOR_PC_IC_H

#include <arch/cpu.h>
#include <qlib.h>

namespace qlib::mediator {

// Intel's 8259A Programmable Interrupt Controller
class PIC {
public:
  using Mask = unsigned char;

  enum {
    MASK_ALL = 0xff,
    MASK_NONE = 0x00,
  };

  enum Mode {
    MASTER,
    SLAVE,
  };

  enum {
    PIC1_CMD = 0x20,
    PIC1_DATA = 0x21,
    PIC2_CMD = 0xA0,
    PIC2_DATA = 0xA1,
  };

  // ICW1: Initialization. Sending ICW1 to the PIC's command port makes it wait
  //       for up to 3 (depending on ICW4_NEEDED) more 8-bit words on it's data
  //       port
  enum ICW1 {
    ICW4_NEEDED = 1 << 0,      // needed?
    SINGLE = 1 << 1,           // single?
    INTERVAL_OF_FOUR = 1 << 2, // interval of 4 | interval of 8
    LEVEL_TRIGGERED = 1 << 3,  // level triggered | edge triggered
    ALWAYS = 1 << 4,           // should always be set
  };

  // ICW2: Vector offset remapping. The value specified here is added to each
  // IRQ
  //       before accessing the Interrupt Descriptor Table (IDT). Note that the
  //       first 32 addresses are reserved by intel to be used for exception
  //       handlers
  enum ICW2 {
    FIRST_AVAILABLE_PIC1 = 0x20,
    FIRST_AVAILABLE_PIC2 = 0x28,
  };

  // ICW3: Cascading, only read if there is more than one PIC in the system
  //       (which is almost always the case). The master PIC interprets the 8
  //       bits as a bitmap for the 8 possible slaves it could handle whilst
  //       the slave interprets it as it's identifier
  enum ICW3 {
    WITHOUT_SLAVES = 0,
  };

  // ICW4: Environment.
  enum ICW4 {
    MODE_8086 = 1 << 0,            // mode 8086/8088 | mode MCS-80/85
    AUTO_EOI = 1 << 1,             // auto eoi | normal eoi
    BUFFERED_MASTER = 1 << 2,      // buffered master | buffered slave
    BUFFERED = 1 << 3,             // buffered?
    SPECIAL_FULLY_NESTED = 1 << 4, // special fully nested?
  };

  // OCW1: Set and clear the Interrupt Mask Register (IMR)

  // OCW2: Send specific/non-specific end of interrupts (EOI). Also configures
  //       automatic/specific rotation of interrupts from the same priority.
  //       Specific commands should use the first three bits to specify the IR
  //       level to be acted upon (0-7)
  enum OCW2 {
    NON_SPECIFIC_EOI = 001 << 5,
    SPECIFIC_EOI = 011 << 5,
    ROTATE_NON_SPECIFIC_EOI = 101 << 5,
    SET_ROTATE_AUTO_EOI = 100 << 5,
    CLEAR_ROTATE_AUTO_EOI = 000 << 5,
    ROTATE_SPECIFIC_EOI = 111 << 5,
    SET_PRIORITY = 110 << 5,
  };

  // OCW3: Special Mask Mode, Poll and status registers
  enum OCW3 {
    READ_IRR = 0xa,
    READ_ISR = 0xb,
  };

  enum IRQ {
    PIT_LINE = 1 << 0,
    KEYBOARD_LINE = 1 << 1,
    CASCADE_LINE = 1 << 2,
    COM2_LINE = 1 << 3,
    COM1_LINE = 1 << 4,
    LPT2_LINE = 1 << 5,
    FLOPPY_LINE = 1 << 6,
    SPURIUS_LINE = 1 << 7,
    RTS_LINE = 1 << 8,
    NIC_LINE = 1 << 9,
    SCSI_LINE = 1 << 10,
    FREE_LINE = 1 << 11,
    PS2_MOUSE_LINE = 1 << 12,
    FPU_LINE = 1 << 13,
    PRIMARY_ATA_LINE = 1 << 14,
    SECONDARY_ATA_LINE = 1 << 15,
  };

  // software caches of both PIC's IMR register
  static inline Mask pic1_mask = MASK_NONE;
  static inline Mask pic2_mask = MASK_NONE;
  static constexpr int LINES_PER_PIC = 8;

  //========INITIALIZE=========================================================//
  // Set up intel's 8259A without cascading in unbuffered, edge-triggered mode.
  // Also Configure interrupt vectors to first available offset (0x20 and
  // 0x28). No auto eoi and not specially fully nested.
  //===========================================================================//

  static void init(void);

  static void eoi(Mode mode = MASTER) {
    CPU::out8(mode == MASTER ? PIC1_CMD : PIC2_CMD, OCW2::NON_SPECIFIC_EOI);
  }

  static CPU::Reg8 imr(Mode mode = MASTER) {
    return (mode == MASTER) ? pic1_mask : pic2_mask;
  }

  static CPU::Reg8 irr(Mode mode = MASTER) {
    CPU::out8((mode == MASTER) ? PIC1_CMD : PIC2_CMD, OCW3::READ_IRR);
    return CPU::in8((mode == MASTER) ? PIC1_CMD : PIC2_CMD);
  }

  static CPU::Reg8 isr(Mode mode = MASTER) {
    CPU::out8((mode == MASTER) ? PIC1_CMD : PIC2_CMD, OCW3::READ_ISR);
    return CPU::in8((mode == MASTER) ? PIC1_CMD : PIC2_CMD);
  }

  static void mask(IRQ line) {
    if (line < LINES_PER_PIC) {
      pic1_mask |= line;
      CPU::out8(PIC1_DATA, pic1_mask);
    } else {
      pic2_mask |= line;
      CPU::out8(PIC2_DATA, pic2_mask);
    }
  }

  static void mask(Mask mask, Mode mode = MASTER) {
    if (mode == MASTER)
      pic1_mask = mask;
    else
      pic2_mask = mask;
  }

  static void unmask(IRQ line) {
    if (line < LINES_PER_PIC) {
      pic1_mask &= ~(line);
      CPU::out8(PIC1_DATA, pic1_mask);
    } else {
      pic2_mask &= ~(line);
      CPU::out8(PIC2_DATA, pic2_mask);
    }
  }
};

} // namespace qlib::mediator

#endif // _QLIB_MEDIATOR_PC_IC_H