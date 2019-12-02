#include <list.hpp>
#include <display.hpp>

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

int utoa(unsigned int v, char * s, unsigned int i);
int itoa(int v, char * s)
{
    unsigned int i = 0;

    if(v < 0) {
        v = -v;
        s[i++] = '-';
    }

    return utoa(static_cast<unsigned int>(v), s, i);
}


int utoa(unsigned int v, char * s, unsigned int i)
{
    unsigned int j;

    if(!v) {
        s[i++] = '0';
        return i;
    }

    for(j = v; j != 0; i++, j /= 10);
    for(j = 0; v != 0; j++, v /= 10)
        s[i - 1 - j] = _digits[v % 10];

    return i;
}

extern "C"
{
void kernel_main(void)
{

	using namespace brae;
	Display cout(QEMU_TEXT_BUFFER);

	List<char> l1;
	List<char> l2;
	List<char> l3;
	l1.push('a');
	l2.push('b');
	l3.push('c');

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
	itoa(l1.head, pointer);
	cout.print(pointer);


	// if (ch1 == ch2 == ch3 == 0)
	// 	cout.putChar('A');
	// else
	// 	cout.putChar('B');
}
}
