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

    void push(T item, P priority) {
        heap.push(std::make_pair(item, priority));
        updatePositionMap();
    }

    T pop() {
        if (heap.empty()) throw std::out_of_range("PriorityQueue Underflow");
        T topItem = heap.peek().first;
        heap.pop();
        updatePositionMap();
        return topItem;
    }

    T top() {
        if (heap.empty()) throw std::out_of_range("PriorityQueue Underflow");
        return heap.peek().first;
    }

    bool empty() {
        return heap.empty();
    }

    int size() {
        return heap.size();
    }

    void clear() {
        heap.clear();
        positionMap.clear();
    }

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