//@TODO: fix this mess!
typedef unsigned short Cell;
static unsigned row = 0;
static unsigned column = 0;
static unsigned char color = (unsigned char) 7;
static Cell * TEXT_BUFFER = (Cell*) 0xB8000;
static const unsigned COLUMNS = 80;
static const unsigned HEIGHT = 25;

extern "C" {
void _putc(char ch) {
    if (ch == '\n') {
		column = 0;
		row++;
		return;
	}

	unsigned index = row * COLUMNS + column;
	TEXT_BUFFER[index] = (Cell) ch | (Cell) color << 8;

	if (++column == COLUMNS) {
		column = 0;
		if (++row == HEIGHT)
			row = 0;
	}
}

void _print(const char * string) {
	for (unsigned i = 0; string[i]; i++)
        _putc(string[i]);
}
}
