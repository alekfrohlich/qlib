#include <iostream>
#include "../../include/list.hpp"

using namespace brae;

int main(void) {
    List<int> l;

	for (int i = 0; i < 10; i++)
        l.push(i);

    while(!l.empty())
        std::cout << l.pop();
}