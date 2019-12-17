#include <machine/pc/vga.h>

namespace qlib::hardware {

int Display::_row = 0;
int Display::_column = 0;
Display::Mode Display::_mode = NORMAL_MODE;
Display::Cell * Display::_buffer = reinterpret_cast<Cell *>(0xB8000);

/*________PRINTER IMPLEMENTATION_____________________________________________*/

void Display::print(const char * s) {
    for (unsigned i = 0; s[i]; i++)
        put(s[i]);
}

void Display::error(void) {
    if (_mode == ERROR_MODE)
        _mode = NORMAL_MODE;
    else
        _mode = ERROR_MODE;
}

/*________INNER WORKINGS_____________________________________________________*/

void Display::put(char c) {
    if (_row == HEIGHT) {
        _row--;
        scroll();
    }
    switch (c) {
        case '\n':
            _column = 0;
            _row++;
            break;

        case '\t':
            print("    ");
            break;

        default:
            unsigned index = _row * WIDTH + _column;
            _buffer[index] = c | _mode;

            if (++_column == WIDTH) {
                _column = 0;
                _row++;
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
    _column = 0;
    _row = 0;
}

void Display::scroll(void) {
    unsigned short c;

    for (int i = 0; i < HEIGHT - 1; i++) {
        for (int j = 0; j < WIDTH; j++) {
            c = _buffer[(i + 1) * WIDTH + j];
            _buffer[i * WIDTH + j] = c;
        }
    }

    char ch = ' ';
    for (int j = 0; j < WIDTH; j++) {
        _buffer[WIDTH * (HEIGHT - 1) + j] = c | _mode;
    }
}

}  // namespace qlib::hardware
