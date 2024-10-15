/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IHeap.h
 * Author: LTSACH
 *
 * Created on 22 August 2020, 07:54
 */

#ifndef IHEAP_H
#define IHEAP_H
#include <string>
using namespace std;

template<class T>
class IHeap {
public:
    virtual ~IHeap(){};
    virtual void push(T item)=0;
    virtual T pop()=0;
    virtual const T peek()=0;
    virtual void remove(T item, void (*removeItemData)(T)=0)=0;
    virtual bool contains(T item)=0;
    virtual int size()=0;
    virtual void heapify(T array[], int size)=0; //build heap from array having size items
    virtual void clear()=0;
    virtual bool empty()=0;
    virtual string toString(string (*item2str)(T&) =0)=0;
};


#endif /* IHEAP_H */

