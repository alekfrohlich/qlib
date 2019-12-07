#include <std/display.h>

using namespace std;

Display std::display;

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

void Display::put(char c) {
    switch (c)
    {
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
                if (++_row == _height)
                _row = 0;
            }
            break;
    }
}

// void Display::erase(void) {
//   for(int i=0;i<_height; i++){
//     for(int j=0;j<_width;j++){

//     }
//   }
// }

// void Display::scroll(void) {
//   _mode ^= error;
// }