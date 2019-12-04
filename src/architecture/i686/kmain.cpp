#include <list.hpp>
#include <display.hpp>

brae::List<char> l1;
brae::List<char> l2;

extern "C"
{
void _pre_kmain(void) {}
void _kmain(void) {
    l1.push('a');
    brae::cout.putChar(l1.pop());
    l2.push('b');
    brae::cout.putChar(l2.pop());
}
}
