#include <arch/cpu.h>
#include <arch/ia32/cpu.h>
#include <lib.h>
#include <machine/pc/8259.h>

#include <std/ostream.h>

/*________DESCRIPTOR TABLES__________________________________________________*/

CPU::IDT_Entry CPU::IDT[IDT_ENTRIES];
CPU::GDT_Entry CPU::GDT[GDT_ENTRIES];

/*________INITIALIZE HARDWARE________________________________________________*/

unsigned char keyboard_map[128] = {
    0,    27,  '1', '2', '3',  '4', '5', '6', '7',  '8', /* 9 */
    '9',  '0', '-', '=', '\b',                           /* Backspace */
    '\t',                                                /* Tab */
    'q',  'w', 'e', 'r',                                 /* 19 */
    't',  'y', 'u', 'i', 'o',  'p', '[', ']', '\n',      /* Enter key */
    0,                                                   /* 29   - Control */
    'a',  's', 'd', 'f', 'g',  'h', 'j', 'k', 'l',  ';', /* 39 */
    '\'', '`', 0,                                        /* Left shift */
    '\\', 'z', 'x', 'c', 'v',  'b', 'n',                 /* 49 */
    'm',  ',', '.', '/', 0,                              /* Right shift */
    '*',  0,                                             /* Alt */
    ' ',                                                 /* Space bar */
    0,                                                   /* Caps lock */
    0,                                                   /* 59 - F1 key ... > */
    0,    0,   0,   0,   0,    0,   0,   0,   0,         /* < ... F10 */
    0,                                                   /* 69 - Num lock*/
    0,                                                   /* Scroll Lock */
    0,                                                   /* Home key */
    0,                                                   /* Up Arrow */
    0,                                                   /* Page Up */
    '-',  0,                                             /* Left Arrow */
    0,    0,                                             /* Right Arrow */
    '+',  0,                                             /* 79 - End key*/
    0,                                                   /* Down Arrow */
    0,                                                   /* Page Down */
    0,                                                   /* Insert Key */
    0,                                                   /* Delete Key */
    0,    0,   0,   0,                                   /* F11 Key */
    0,                                                   /* F12 Key */
    0, /* All other keys are undefined */
};

extern "C" {
void keyboard_handler_main(void) {
  // end of interrupt
  CPU::out8(0x20, 0x20);
  unsigned char status;
  char keycode;

  /* write EOI */
  CPU::out8(0x20, 0x20);

  status = CPU::in8(0x64); // status port
  /* Lowest bit of status will be set if buffer is not empty */
  if (status & 0x01) {
    keycode = CPU::in8(0x60); // data port
    if (keycode < 0)
      return;
    std::cout << (char)keyboard_map[(unsigned char)keycode];
  }
}
void keyboard_handler(void);
}

static void set_gdte(int n, unsigned base, unsigned limit, unsigned granularity,
                     unsigned access) {
  CPU::GDT[n].base_low = (base & 0xffffff);
  CPU::GDT[n].base_high = (base >> 24) & 0xff;
  CPU::GDT[n].limit_low = (limit & 0xffff);
  CPU::GDT[n].limit_high = (limit >> 16) & 0xf;
  CPU::GDT[n].granularity = granularity;
  CPU::GDT[n].access = access;
}

static void set_idte(int n, unsigned selector, unsigned type, unsigned isr) {
  CPU::IDT[n].offset_low = isr & 0xffff;
  CPU::IDT[n].selector = selector;
  CPU::IDT[n].zero = 0;
  CPU::IDT[n].type = type;
  CPU::IDT[n].offset_high = (isr & 0xffff0000) >> 16;
}

void CPU::init() {
  // create tree entries: NULL, CODE, DATA
  set_gdte(0, 0, 0x00000, 0x0, 0x00);
  set_gdte(1, 0, 0xfffff, 0xc, 0x9A);
  set_gdte(2, 0, 0xfffff, 0xc, 0x92);

  // load gdtr and reload segment registers
  Reg16 size = sizeof(GDT) - 1;
  Reg32 ptr = reinterpret_cast<Reg32>(GDT);
  load_gdt(size, ptr);

  // create IDT entry for keyboard interrupts
  Reg32 keyboard_address = reinterpret_cast<LogicalAddr>(keyboard_handler);
  set_idte(0x21, 0x08, 0x8e, keyboard_address);

  // load idtr
  size = sizeof(IDT) - 1;
  ptr = reinterpret_cast<Reg32>(IDT);
  load_idt(size, ptr);

  PIC::init();

  int_enable();

  PIC::unmask(PIC::KEYBOARD_LINE);
}
