#ifndef __QLIB_LIST_H
#define __QLIB_LIST_H

#include <system.h>

namespace qlib {

// Round Robin List
// Implemented as Doubly-Linked Circular List
template<typename T>
class RR_List
// @TODO: Fix memory leaks after implementing proper heap
{
 public:
    RR_List() : size(0), head(nullptr) {}
    T * choose();
    T * chosen();
    void insert(T *);
    T * drop_chosen();

 private:
    struct Node {
        Node(T * element) : prev(this), next(this), element(element) {}
        Node(Node * prev, Node * next, T * element)
            : prev(prev), next(next), element(element) {}
        Node * next;
        Node * prev;
        T * element;
    };
    int size;
    Node * head;
};

// Move chosen
template<typename T>
T * RR_List<T>::choose() {
    head = head->next;
    return head->element;
}

// Return chosen
template<typename T>
T * RR_List<T>::chosen() {
    return head->element;
}

// Insert before chosen
template<typename T>
void RR_List<T>::insert(T * element) {
    if (size == 0) {
        head = new (SYSTEM) Node(element);
        size++;
    } else {
        Node * new_node = new (SYSTEM) Node(head->prev, head, element);
        auto sentinel = head->prev;
        head->prev = new_node;
        sentinel->next = new_node;
        size++;
    }
}

// Removes and returns chosen
template<typename T>
T * RR_List<T>::drop_chosen() {
    if (size == 0) {
        return nullptr;
    } else if (size == 1) {
        auto chosen = head->element;
        head = nullptr;
        size--;
        return chosen;
    } else {
        auto chosen = head->element;
        head->prev->next = head->next;
        head->next->prev = head->prev;
        head = head->next;
        size--;
        return chosen;
    }
}
}  // namespace qlib
#endif  // __QLIB_LIST_H
