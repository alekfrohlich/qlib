#include <machine/pc/display.h>

using namespace std;

// standard x86 vga-display
Display std::display;

/*________PRINTER IMPLEMENTATION_____________________________________________*/

void Display::print(const char * s) {
    for (unsigned i = 0; s[i]; i++)
        put(s[i]);
}

void Display::error(void) {
    if (_mode == _error)
        _mode = _normal;
    else
        _mode = _error;
}

/*________INNER WORKINGS_____________________________________________________*/

void Display::put(char c) {
    if (_row == _height) {
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
            unsigned index = _row * _width + _column;
            _buffer[index] = c | _mode;

            if (++_column == _width) {
                _column = 0;
                _row++;
            }
            break;
    }
}

void Display::erase(void) {
    char c = ' ';
    for (int i = 0; i < _height; i++) {
        for (int j = 0; j < _width; j++) {
            put(c);
        }
    }
    _column = 0;
    _row = 0;
}

void Display::scroll(void) {
    unsigned short c;

    for (int i = 0; i < _height - 1; i++) {
        for (int j = 0; j < _width; j++) {
            c = _buffer[(i + 1) * _width + j];
            _buffer[i * _width + j] = c;
        }
    }

    char ch = ' ';
    for (int j = 0; j < _width; j++) {
        _buffer[_width * (_height - 1) + j] = c | _mode;
    }
}