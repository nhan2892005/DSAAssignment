#ifndef QUEUE_H
#define QUEUE_H
#include "list/DLinkedList.h"
template<class T>
class Queue {
protected:
    DLinkedList<T> list;
public:
    Queue(  void (*deleteUserData)(DLinkedList<T> *) = 0,
            bool (*itemEqual)(T &, T &) = 0)
        :list(deleteUserData, itemEqual){}
        
    void push(T item) {
        this->list.add(item);
    }

    T pop() {
        if (this->empty()) throw std::out_of_range("Empty Queue");
        return this->list.removeAt(0);
    }

    T top() {
        if (this->empty()) throw std::out_of_range("Empty Queue");
        return this->list.get(0);
    }

    bool empty() {
        return this->list.empty();
    }

    int size() {
        return this->list.size();
    }

    void clear() {
        return this->list.clear();
    }
};

#endif /* QUEUE_H */