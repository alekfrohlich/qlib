#ifndef __QLIB_LIST_H
#define __QLIB_LIST_H

#include <system.h>

// @TODO: fix memory leaks after implementing proper heap
template<typename T>
class List
{
public:
    List() : _size(0), head(nullptr) {}

    void push_back(T * data) {
        head = new (SYSTEM) Node(data, head);
        _size++;
    }

    T * pop_back() {
        if (size() == 0)
            return nullptr;
        auto dead = head->data;
        head = head->next;
        _size--;
        return dead;
    }

    T * pop_front() {
        if (size() == 0)
            return nullptr;
        if (size() == 1) {
            _size--;
            auto dead = head->data;
            head = nullptr;
            return dead;
        }

        auto it = head;
        while(it->next->next)
            it = it->next;

        _size--;
        auto dead = it->next->data;
        it->next = nullptr;
        return dead;
    }

    int size() { return _size; }

private:
    struct Node {
        Node(T * data, Node * next) : data(data), next(next) {}
        T * data;
        Node * next;
    };

    int _size;
    Node * head;
};

#endif  // __QLIB_LIST_H