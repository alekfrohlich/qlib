#ifndef BRAE_DISPLAY_H
#define BRAE_DISPLAY_H

#include <stddef.h>
#include <stdint.h>

namespace brae {

//@FIX-ME: properly handle CTOR_LIST and DTOR_LIST
// Display cout(QEMU_TEXT_BUFFER);
uint16_t * QEMU_TEXT_BUFFER = (uint16_t*) 0xB8000;

// hardware text mode color constants
enum VGAColor {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    BROWN = 6,
    LIGHT_GREY = 7,
    DARK_GREY = 8,
    LIGHT_BLUE = 9,
    LIGHT_GREEN = 10,
    LIGHT_CYAN = 11,
    LIGHT_RED = 12,
    LIGHT_MAGENTA = 13,
    LIGHT_BROWN = 14,
    WHITE = 15,
};

static inline uint8_t vga_entry_color(enum VGAColor fg, enum VGAColor bg)
{
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

//@TODO: add pixelmap and other template parameters
class Display {
  public:
    Display(uint16_t * address);
    ~Display();

    void setColor(enum VGAColor color);
    void print(const char * string);
    void putChar(char ch);

  private:
    static const size_t VGA_WIDTH = 80;
    static const size_t VGA_HEIGHT = 25;

    size_t row;
    size_t column;
    uint8_t color;
    uint16_t * address;
};

}; // namespace brae

brae::Display::Display(uint16_t * address) {
	this->row = 0;
	this->column = 0;
	this->color = vga_entry_color(LIGHT_GREY, BLACK);
	this->address = address;

	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			address[index] = vga_entry(' ', color);
		}
	}
}

//@TODO: delete buffer
brae::Display::~Display() {}

//@TODO: create brea::string, remove putChar?
void brae::Display::print(const char * string) {
	for (size_t i = 0; string[i]; i++)
        putChar(string[i]);
}

void brae::Display::setColor(enum VGAColor color) {
    this->color = color;
}

void brae::Display::putChar(char ch) {
if (ch == '\n') {
		column = 0;
		row++;
		return;
	}

	size_t index = row * VGA_WIDTH + column;
	this->address[index] = vga_entry(ch, color);

	if (++column == VGA_WIDTH) {
		column = 0;
		if (++row == VGA_HEIGHT)
			row = 0;
	}
}

#endif // BRAE_DISPLAY_H
