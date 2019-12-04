#ifndef BRAE_LIST_H
#define BRAE_LIST_H

#include <display.hpp>

namespace brae {

//@TODO: add additional methods for queues, linkedlists...
template<typename T>
class List {
  public:
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

}; // namespace brae

template<typename T>
brae::List<T>::List() {
  head = 0;
  for (int i = 0; i < 10; i++)
      vector[i] = 0;
}

template<typename T>
brae::List<T>::~List() {}

template<typename T>
void brae::List<T>::push(T arg0) {
  cout.putChar((char) arg0);
  vector[head++] = arg0;
}

template<typename T>
T brae::List<T>::pop() { return vector[--head]; }

template<typename T>
T brae::List<T>::top() const { return vector[head]; }

template<typename T>
bool brae::List<T>::empty() const { return head == 0; }

#endif // BRAE_LIST_H
