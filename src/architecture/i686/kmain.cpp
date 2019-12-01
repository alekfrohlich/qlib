#include <list.hpp>
#include <display.hpp>

using namespace brae;
List<char> l1;
List<char> l2;

// ignore dumb g++
extern "C" {
	void * __dso_handle = 0;
	void * __cxa_atexit = 0;
}

const char _digits[] = "0123456789abcdef";

int ptoa(const void * p, char * s)
{
    unsigned int j, v = reinterpret_cast<unsigned int>(p);

    s[0] = '0';
    s[1] = 'x';

    for(j = 0; j < sizeof(void *) * 2; j++, v >>= 4)
        s[2 + sizeof(void *) * 2 - 1 - j]
            = _digits[v & 0xf];

    return j + 2;
}

extern "C"
{
void kernel_main(void)
{
	List<char> l3;

	l1.push('a');
	l2.push('b');
	l3.push('c');

	Display cout(QEMU_TEXT_BUFFER);
	// cout.print("Welcome to brae!\n" \
	// 					 "Developed by Alek Frohlich & Nicolas Goeldner\n");

	char pointer[8];
	ptoa(&l3, pointer);
	cout.print(pointer);

	cout.print("BEGIN");

	char ch1 = l1.pop();
	char ch2 = l2.pop();
	char ch3 = l3.pop();

	cout.putChar(ch3);
	cout.putChar(ch3);
	cout.putChar(ch3);

	cout.putChar(ch1);
	cout.putChar(ch1);
	cout.putChar(ch1);

	cout.putChar(ch2);
	cout.putChar(ch2);
	cout.putChar(ch2);

	cout.print("END\n");

	// if (ch1 == ch2 == ch3 == 0)
	// 	cout.putChar('A');
	// else
	// 	cout.putChar('B');
}
}
