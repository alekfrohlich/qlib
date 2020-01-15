#include <machine/pc/vga.h>

namespace qlib::hardware {

// void VGA::default_init() {
//     CPU::out8(0x3d4, 0x0a);
// 	CPU::out8(0x3d5, 0x00);
// 	CPU::out8(0x3d4, 0x0b);
// 	CPU::out8(0x3d5, 0x1f);
// }

/*________PRINTER IMPLEMENTATION_____________________________________________*/

void Display::print(const char * s) {
    for (unsigned i = 0; s[i]; i++)
        put(s[i]);
}

void Display::error(void) {
    if (mode == Mode::ERROR)
        mode = Mode::NORMAL;
    else
        mode = Mode::ERROR;
}

/*________INNER WORKINGS_____________________________________________________*/

void Display::put(char c) {
    if (row == HEIGHT) {
        row--;
        scroll();
    }
    switch (c) {
        case '\n':
            column = 0;
            row++;
            break;

        case '\t':
            print("    ");
            break;

        default:
            auto index = row * WIDTH + column;

            // @TODO: consider mode
            buffer[index] = mode | c;

            if (++column == WIDTH) {
                column = 0;
                row++;
            }
            break;
    }
}

void Display::erase(void) {
    char c = ' ';
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            put(c);
        }
    }
    column = 0;
    row = 0;
}

void Display::scroll(void) {
    unsigned short c;

    for (int i = 0; i < HEIGHT - 1; i++) {
        for (int j = 0; j < WIDTH; j++) {
            c = buffer[(i + 1) * WIDTH + j];
            buffer[i * WIDTH + j] = c;
        }
    }

    char ch = ' ';
    for (int j = 0; j < WIDTH; j++) {
        buffer[WIDTH * (HEIGHT - 1) + j] = c | mode;
    }
}

}  // namespace qlib::hardware
