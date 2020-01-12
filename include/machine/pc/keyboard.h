#ifndef _QLIB_HARDWARE_KEYBOARD_H
#define _QLIB_HARDWARE_KEYBOARD_H

#include <machine/pc/pic.h>
#include <qlib.h>

#include <qlib/ostream.h>

namespace qlib::hardware {

// 8042 PS/2 keyboard controller
class Keyboard
{
 public:
    static constexpr unsigned char keyboard_map[128] = {
        0, 27, '1', '2', '3', '4', '5', '6', '7', '8',     // 9
        '9', '0', '-', '=', '\b',                          // backspace
        '\t',                                              // tab
        'q', 'w', 'e', 'r',                                // 19
        't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',      // enter key
        0,                                                 // 29 - control
        'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',  // 39
        '\'', '`', 0,                                      // left shift
        '\\', 'z', 'x', 'c', 'v', 'b', 'n',                // 49
        'm', ',', '.', '/', 0,                             // right shift
        '*', 0,                                            // alt
        ' ',                                               // space bar
        0,                                                 // caps lock
        0,                                                 // 59 - F1 key ... >
        0, 0, 0, 0, 0, 0, 0, 0, 0,                         // < ... F10
        0,                                                 // 69 - num lock
        0,                                                 // scroll lock
        0,                                                 // home key
        0,                                                 // up arrow
        0,                                                 // page up
        '-', 0,                                            // left arrow
        0, 0,                                              // right arrow
        '+', 0,                                            // 79 - end key
        0,                                                 // down arrow
        0,                                                 // page down
        0,                                                 // insert key
        0,                                                 // delete key
        0, 0, 0, 0,                                        // F11 key
        0,                                                 // F12 key
        0,  // all other keys are undefined
    };

    //========DEFAULT INIT=======================================================//
    //
    //===========================================================================//

    static void default_init(void);

    // @TODO: this should not print (should buffer instead?)
    INTRIN void default_handler(void) {
        unsigned char status;
        char keycode;

        PIC::eoi(PIC::MASTER);

        status = CPU::in8(0x64);  // status port
        /* Lowest bit of status will be set if buffer is not empty */
        if (status & 0x01) {
            keycode = CPU::in8(0x60);  // data port
            if (keycode < 0) return;
            qlib::cout << (char) keyboard_map[(unsigned char) keycode];
        }
    }
};

};  // namespace qlib::hardware

#endif  // _QLIB_HARDWARE_KEYBOARD_H
