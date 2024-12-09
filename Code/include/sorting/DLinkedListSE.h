/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DLinkedListSE.h
 * Author: LTSACH
 *
 * Created on 31 August 2020, 14:13
 */

#ifndef DLINKEDLISTSE_H
#define DLINKEDLISTSE_H
#include "list/DLinkedList.h"
#include "sorting/ISort.h"

template<class T>
class DLinkedListSE: public DLinkedList<T>{
public:
    
    DLinkedListSE(
            void (*removeData)(DLinkedList<T>*)=0, 
            bool (*itemEQ)(T&, T&)=0 ) : 
            DLinkedList<T>(removeData, itemEQ){
    }
    
    DLinkedListSE(const DLinkedList<T>& list){
        this->copyFrom(list);
    }

    void merge(DLinkedListSE<T>& list1, DLinkedListSE<T>& list2, int (*comparator)(T&,T&)=0){
        this->clear(); // Clear the current list

        typename DLinkedList<T>::Iterator it1 = list1.begin();
        typename DLinkedList<T>::Iterator it2 = list2.begin();

        while (it1 != list1.end() && it2 != list2.end()) {
            if (compare(*it1, *it2, comparator) <= 0) {
                this->add(*it1);
                ++it1;
            } else {
                this->add(*it2);
                ++it2;
            }
        }

        // Add remaining elements
        while (it1 != list1.end()) {
            this->add(*it1);
            ++it1;
        }

        while (it2 != list2.end()) {
            this->add(*it2);
            ++it2;
        }
    }
    
    void sort(int (*comparator)(T&,T&)=0){
        //TODO: implement this function
        //     - You should implement the merge sort algorithm
        //     - You should use the merge function above
        if (this->size() <= 1) return; // Base case: already sorted

        DLinkedListSE<T> leftList;
        DLinkedListSE<T> rightList;

        int middle = this->size() / 2;
        typename DLinkedList<T>::Iterator it = this->begin();

        for (int i = 0; i < middle; ++i) {
            leftList.add(*it);
            ++it;
        }

        for (int i = middle; i < this->size(); ++i) {
            rightList.add(*it);
            ++it;
        }

        leftList.sort(comparator);
        rightList.sort(comparator);

        this->merge(leftList, rightList, comparator);
    };
    
protected:
    static int compare(T& lhs, T& rhs, int (*comparator)(T&,T&)=0){
        if(comparator != 0) return comparator(lhs, rhs);
        else{
            if(lhs < rhs) return -1;
            else if(lhs > rhs) return +1;
            else return 0;
        }
    }
};

#endif /* DLINKEDLISTSE_H */

