#ifndef STACK_H
#define STACK_H
#include "list/DLinkedList.h"
#include "stacknqueue/IDeck.h"

template<class T>
class Stack {
protected:
    DLinkedList<T> list;

    bool (*itemEqual)(T &lhs, T &rhs);        // function pointer: test if two items (type: T&) are equal or not
    void (*deleteUserData)(DLinkedList<T> *); // function pointer: be called to remove items (if they are pointer type)

public:
    Stack(  void (*deleteUserData)(DLinkedList<T> *) = 0,
            bool (*itemEqual)(T &, T &) = 0)
        :list(deleteUserData, itemEqual){}

    void push(T item) {
        this->list.add(0, item);
    }

    T pop() {
        if (this->list.empty()) throw Underflow("Stack");
        return this->list.removeAt(0);
    }

    T top() {
        if (this->list.empty()) throw Underflow("Stack");
        return this->list.get(0);
    }

    bool empty() {
        return this->list.empty();
    }

    int size() {
        return this->list.size();
    }

    void clear() {
        this->list.clear();
    }
};

#endif