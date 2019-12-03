#include <list.hpp>
#include <display.hpp>

// ignore dumb g++
extern "C" {
	void * __dso_handle = 0;
	void * __cxa_atexit = 0;
}

// const char _digits[] = "0123456789abcdef";

// int ptoa(const void * p, char * s)
// {
//     unsigned int j, v = reinterpret_cast<unsigned int>(p);

//     s[0] = '0';
//     s[1] = 'x';

//     for(j = 0; j < sizeof(void *) * 2; j++, v >>= 4)
//         s[2 + sizeof(void *) * 2 - 1 - j]
//             = _digits[v & 0xf];

//     return j + 2;
// }

// int utoa(unsigned int v, char * s, unsigned int i);
// int itoa(int v, char * s)
// {
//     unsigned int i = 0;

//     if(v < 0) {
//         v = -v;
//         s[i++] = '-';
//     }

//     return utoa(static_cast<unsigned int>(v), s, i);
// }


// int utoa(unsigned int v, char * s, unsigned int i)
// {
//     unsigned int j;

//     if(!v) {
//         s[i++] = '0';
//         return i;
//     }

//     for(j = v; j != 0; i++, j /= 10);
//     for(j = 0; v != 0; j++, v /= 10)
//         s[i - 1 - j] = _digits[v % 10];

//     return i;
// }

extern "C"
{
void kernel_main(void)
{

	using namespace brae;
	Display cout(TEXT_BUFFER);

	List<char> l1;

	l1.push('a');
	cout.putChar(l1.pop());
}
}
