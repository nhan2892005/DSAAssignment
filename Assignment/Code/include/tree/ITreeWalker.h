/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ITreeWalker.h
 * Author: LTSACH
 *
 * Created on 20 August 2020, 15:05
 */

#ifndef ITREEWALKER_H
#define ITREEWALKER_H
#include "list/DLinkedList.h"

template<class K, class V>
class ITreeWalker{
public:
    virtual ~ITreeWalker(){};
    virtual DLinkedList<V> dfs()=0;
    virtual DLinkedList<V> bfs()=0;
    virtual DLinkedList<K> bfsKey()=0;
    virtual DLinkedList<V> nlr()=0;
    virtual DLinkedList<V> lrn()=0;
    virtual DLinkedList<V> lnr()=0;
};




#endif /* ITREEWALKER_H */

