#ifndef LIST_H
#define LIST_H

namespace std {

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

}; // namespace std

template<typename T>
std::List<T>::List() {
  head = 0;
  for (int i = 0; i < 10; i++)
      vector[i] = 0;
}

template<typename T>
std::List<T>::~List() {}

template<typename T>
void std::List<T>::push(T arg0) { vector[head++] = arg0; }

template<typename T>
T std::List<T>::pop() { return vector[--head]; }

template<typename T>
T std::List<T>::top() const { return vector[head]; }

template<typename T>
bool std::List<T>::empty() const { return head == 0; }

#endif // LIST_H
