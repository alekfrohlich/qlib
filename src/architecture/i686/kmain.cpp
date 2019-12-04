#include <list.hpp>
#include <display.hpp>

brae::Display cout(brae::TEXT_BUFFER);
brae::List<char> l1;
brae::List<char> l2;

extern "C"
{
void _pre_kmain(void) {}
void _kmain(void) {
    cout.putChar('a');
    l1.push('b');
    l2.push('c');
    cout.putChar(l1.pop());
    cout.putChar(l2.pop());
}
}
