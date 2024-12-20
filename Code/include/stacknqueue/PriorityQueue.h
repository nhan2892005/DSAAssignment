#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include "heap/Heap.h"
#include <unordered_map>

template<class T, class P>
class PriorityQueue {
private:
    Heap<std::pair<T, P>> heap;
    std::unordered_map<T, int> positionMap;

    static int comparator(std::pair<T, P>& lhs, std::pair<T, P>& rhs) {
        if (lhs.second < rhs.second) return -1;
        if (lhs.second > rhs.second) return 1;
        return 0;
    }

    void updatePositionMap() {
        for (int i = 0; i < heap.size(); ++i) {
            positionMap[heap.elements[i].first] = i;
        }
    }

public:
    PriorityQueue() : heap(comparator) {}

    /*
    ! push(item: T, priority: P)
    ? Functional:
        * Add item to the queue with the given priority
    ? Parameters:
        * item: T - The item to be added
        * priority: P - The priority of the item
    ? Return:
        * void
    */
    void push(T item, P priority) {
        heap.push(std::make_pair(item, priority));
        updatePositionMap();
    }

    /*
    ! pop()
    ? Functional:
        * Remove and return the item with the highest priority
    ? Parameters:
        * None
    ? Return:
        * T - The item with the highest priority
    */
    T pop() {
        if (heap.empty()) throw std::out_of_range("PriorityQueue Underflow");
        T topItem = heap.peek().first;
        heap.pop();
        updatePositionMap();
        return topItem;
    }

    /*
    ! top()
    ? Functional:
        * Return the item with the highest priority
    ? Parameters:
        * None
    ? Return:
        * T - The item with the highest priority
    */
    T top() {
        if (heap.empty()) throw std::out_of_range("PriorityQueue Underflow");
        return heap.peek().first;
    }

    /*
    ! empty()
    ? Functional:
        * Check if the queue is empty
    ? Parameters:
        * None
    ? Return:
        * bool - True if the queue is empty, False otherwise
    */
    bool empty() {
        return heap.empty();
    }

    /*
    ! size()
    ? Functional:
        * Return the number of items in the queue
    ? Parameters:
        * None
    ? Return:
        * int - The number of items in the queue
    */
    int size() {
        return heap.size();
    }

    /*
    ! clear()
    ? Functional:
        * Remove all items in the queue
    ? Parameters:
        * None
    ? Return:
        * void
    */
    void clear() {
        heap.clear();
        positionMap.clear();
    }

    /*
    ! update(item: T, priority: P)
    ? Functional:
        * Update the priority of the item
    ? Parameters:
        * item: T - The item to be updated
        * priority: P - The new priority of the item
    ? Return:
        * void
    */
    void update(T item, P priority) {
        int index = positionMap[item];
        P oldPriority = heap.elements[index].second;
        heap.elements[index].second = priority;
        if (priority < oldPriority) {
            heap.reheapUp(index);
        } else {
            heap.reheapDown(index);
        }
        updatePositionMap();
    }
};

#endif /* PRIORITYQUEUE_H */