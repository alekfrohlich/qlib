#ifndef LIST_H
#define LIST_H

namespace qlib {

template<typename T>
class List
{
 public:
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
    // @TODO: Add additional methods for queues, linkedlists...
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
    List();
    ~List();
    void push(T arg0);
    T pop();
    T top() const;
    bool empty() const;

    int head;

 private:
    T vector[10];
};

};  // namespace qlib

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
// @TODO: Move implementation code to .cc file
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

template<typename T>
qlib::List<T>::List() {
    head = 0;
    for (int i = 0; i < 10; i++)
        vector[i] = 0;
}

template<typename T>
qlib::List<T>::~List() {
}

template<typename T>
void qlib::List<T>::push(T arg0) {
    vector[head++] = arg0;
}

template<typename T>
T qlib::List<T>::pop() {
    return vector[--head];
}

template<typename T>
T qlib::List<T>::top() const {
    return vector[head];
}

template<typename T>
bool qlib::List<T>::empty() const {
    return head == 0;
}

#endif  // LIST_H
