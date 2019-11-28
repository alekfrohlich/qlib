#ifndef STRUCTURES_LINKED_STACK_H
#define STRUCTURES_LINKED_STACK_H

namespace structures {

template<typename T>
class StaticStack {
  public:
    StaticStack();
    ~StaticStack();
    void push(T arg0);
    T pop();
    T top() const;
    bool empty() const;

  private:
    int head{0};
    T vector[10] = {0,};
};

};

template<typename T>
structures::StaticStack<T>::StaticStack() {}

template<typename T>
structures::StaticStack<T>::~StaticStack() {}

template<typename T>
void structures::StaticStack<T>::push(T arg0) { vector[this->head++] = arg0; }

template<typename T>
T structures::StaticStack<T>::pop() { return this->vector[this->head--]; }

template<typename T>
T structures::StaticStack<T>::top() const { return this->vector[this->head]; }

template<typename T>
bool structures::StaticStack<T>::empty() const { return head == 0; }

#endif  // STRUCTURES_LINKED_STACK_H