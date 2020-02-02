#include <iostream>

char array[32] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i'};

int main(void) {
    using std::cout;
    char * ptr = array;
    cout << *ptr << "\n";
    ptr += 4;
    cout << *ptr << "\n";
}