/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BSTDemo.h
 * Author: LTSACH
 *
 * Created on 20 August 2020, 15:34
 */

#ifndef BSTDEMO_H
#define BSTDEMO_H

#include "tree/BST.h"
#include <math.h>


void bstDemo1(){
    BST<int, int*> tree;
    int values[] = {50, 20, 80, 15, 60, 90, 7, 12};
    for(int idx=0; idx <8; idx++)
        tree.add(values[idx]);
    cout << "#nodes: " << tree.size() << endl;
    cout << "height: " << tree.height() << endl;
    tree.println();
}

void bstDemo2(){
    BST<int, int*> tree;
    int values[] = {50, 20, 80, 15, 60, 90, 7, 12, 30, 40, 25, 35, 85};
    for(int idx=0; idx < 13; idx++)
        tree.add(values[idx]);
    cout << "#nodes: " << tree.size() << endl;
    cout << tree.toString() << endl;
    cout << endl;
    
    tree.remove(50);
    cout << "#nodes: " << tree.size() << endl;
    cout << tree.toString() << endl;
}
void bstDemo2_1(){
    BST<int, int*> tree;
    int values[] = {50, 20, 80, 15, 60, 90, 7, 12, 30, 40, 25, 35, 85};
    for(int idx=0; idx < 13; idx++)
        tree.add(values[idx], new int(values[idx]));
    cout << "#nodes: " << tree.size() << endl;
    cout << tree.toString() << endl;
    cout << endl;
    
    //O(n^2)
    DLinkedList<int*> list = tree.ascendingList();
    for(int idx=0; idx < list.size(); idx++){
        cout << (list.get(idx)) << ", ";
    }
    cout << endl;
    
    /*: Linear O(n)
    DLinkedList<int*>::Iterator it;
    for(it = list.begin(); it != list.end(); it++){
        int v = **it;
        cout << v << ", ";
    }
    cout << endl;
    */
}

void bstDemo3(){
    BST<float, Point*> tree;
    Point* p1 = new Point(62.1, 23.2);
    Point* p2 = new Point(12.1, 13.22);
    Point* p3 = new Point(72.21, 13.62);
    Point* p4 = new Point(2.51, 3.25);
    tree.add(p1->getX(), p1);
    tree.add(p2->getX(), p2);
    tree.add(p3->getX(), p3);
    tree.add(p4->getX(), p4);
            
    cout << "#nodes: " << tree.size() << endl;
    cout << tree.toString() << endl;
}
void bstDemo4(){
    BST<float, Point*> tree;
    Point* p1 = new Point(62.1, 23.2);
    Point* p2 = new Point(12.1, 13.22);
    Point* p3 = new Point(72.21, 13.62);
    Point* p4 = new Point(2.51, 3.25);
    tree.add(p1->getX(), p1);
    tree.add(p2->getX(), p2);
    tree.add(p3->getX(), p3);
    tree.add(p4->getX(), p4);
            
    cout << "#nodes: " << tree.size() << endl;
    cout << tree.toString() << endl;
    cout << "search with key = " << p1->getX() << endl;
    bool found;
    Point* pF = tree.search(100, found);
    cout << "Result: " << (found ? "Found" : "Not found") << endl;
}

void bstDemo5(){
    BST<float, Point*> tree;
    Point* p1 = new Point(62.1, 23.2);
    Point* p2 = new Point(12.1, 13.22);
    Point* p3 = new Point(72.21, 13.62);
    Point* p4 = new Point(2.51, 3.25);
    tree.add(p1->getX(), p1);
    tree.add(p2->getX(), p2);
    tree.add(p3->getX(), p3);
    tree.add(p4->getX(), p4);
            
    cout << "#nodes: " << tree.size() << endl;
    cout << tree.toString() << endl;
    DLinkedList<Point*> list = tree.bfs();
    cout << "Result:" << endl;
    DLinkedList<Point*>::Iterator it = list.begin();
    while(it != list.end()){
        cout << **it << endl;
        it++;
    }
    cout << endl;
}

void bstDemo6(){
    BST<int, int*> tree;
    int values[] = {50, 20, 80, 15, 60, 90, 7, 12};
    for(int idx=0; idx <8; idx++)
        tree.add(values[idx], new int(values[idx]));
    cout << "#nodes: " << tree.size() << endl;
    cout << tree.toString() << endl;
    
    DLinkedList<int*> list = tree.lnr();
    cout << "Result:" << endl;
    DLinkedList<int*>::Iterator it = list.begin();
    while(it != list.end()){
        cout << **it << endl;
        it++;
    }
    cout << endl;
}

void bstDemo7(){
    BST<int, int*> tree;
    int values[] = {50, 20, 80, 15, 60, 90, 7, 12};
    for(int idx=0; idx <8; idx++)
        tree.add(values[idx], new int(values[idx]) );
    cout << "#nodes: " << tree.size() << endl;
    cout << tree.toString() << endl;
    
    cout << "Height:" << tree.height() << endl;
}

void bstDemo8(){
    BST<int, int*> tree;
    for(int idx=0; idx <1000; idx++) tree.add(idx);
    cout << "BST: #nodes: " << tree.size() << endl;
    cout << "BST: height: " << tree.height() << endl;
    //tree.println();
}


#endif /* BSTDEMO_H */

