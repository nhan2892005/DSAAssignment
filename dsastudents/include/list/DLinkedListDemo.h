/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DLinkedListDemo.h
 * Author: LTSACH
 *
 * Created on 19 August 2020, 21:34
 */

#ifndef DLINKEDLISTDEMO_H
#define DLINKEDLISTDEMO_H

#include <iostream>
#include <iomanip>
#include "list/DLinkedList.h"
#include "util/Point.h"
using namespace std;

void dlistDemo1(){
    List<int> dlist;
    for(int i = 0; i< 20 ; i++)
        dlist.add(i, i*i);
    dlist.println();
    
    for(List<int>::Iterator it=dlist.begin(); it != dlist.end(); it++ )
        cout << *it << " ";
    cout << endl;
}
void dlistDemo2(){
    DLinkedList<Point*> list1(&DLinkedList<Point*>::free, &Point::pointEQ);
    list1.add(new Point(23.2f, 25.4f));
    list1.add(new Point(24.6f, 23.1f));  
    list1.add(new Point(12.5f, 22.3f)); 
    
    for(DLinkedList<Point*>::Iterator it = list1.begin(); it != list1.end(); it++)
        cout << **it << endl;
    
    Point* p1 = new Point(24.6f, 23.1f); //found in list
    Point* p2 = new Point(124.6f, 23.1f); //not found
    cout << *p1 << "=> " << (list1.contains(p1)? "found; " : "not found; ")
                << " indexOf returns: " << list1.indexOf(p1) << endl;
    cout << *p2 << "=> " << (list1.contains(p2)? "found; " : "not found; ")
                << " indexOf returns: " << list1.indexOf(p2) << endl;
    
    ///Different results if not pass &Point::equals
    cout << endl << endl;
    DLinkedList<Point*> list2(&DLinkedList<Point*>::free);
    list2.add(new Point(23.2f, 25.4f));
    list2.add(new Point(24.6f, 23.1f));  
    list2.add(new Point(12.5f, 22.3f)); 
    
    for(DLinkedList<Point*>::Iterator it = list2.begin(); it != list2.end(); it++)
        cout << **it << endl;
    
    cout << *p1 << "=> " << (list2.contains(p1)? "found; " : "not found; ")
                << " indexOf returns: " << list2.indexOf(p1) << endl;
    cout << *p2 << "=> " << (list2.contains(p2)? "found; " : "not found; ")
                << " indexOf returns: " << list2.indexOf(p2) << endl;
    
    delete p1; delete p2;
}

void dlistDemo3(){
    DLinkedList<Point> dList;
    dList.add(Point(1.5, 3.5));
    dList.add(Point(2.5, 4.5));
    dList.add(Point(1.6, 3.1));
    
    cout << "test for indexOf: " << endl;
    Point p(1.6, 3.1);
    cout << p << " at: " << dList.indexOf(p);
}
bool pointComparator(Point*& p1, Point*& p2){
    return (p1->getX() == p2->getX()) && (p1->getY() == p2->getY());
}
string LpointPtr2Str(Point*& ptr){
    stringstream os;
    os << "("   << ptr->getX() << ", " 
                << ptr->getY()
       << ")";
    return os.str();
}
void dlistDemo4(){
    DLinkedList<Point*> dList(&DLinkedList<Point*>::free, &pointComparator);
    dList.add(new Point(1.5, 3.5));
    dList.add(new Point(2.5, 4.5));
    dList.add(new Point(1.6, 3.1));
    dList.println(&LpointPtr2Str);
    
    cout << "test for indexOf: " << endl;
    Point* p = new Point(1.6, 3.1);
    cout << *p << " at: " << dList.indexOf(p) << endl;
    delete p;
}
void dlistDemo5(){
    DLinkedList<float> dList;
    dList.add(3.2);
    dList.add(5.5);
    dList.println();
    cout << "index of 5.5: " << dList.indexOf(5.5) << endl;
    cout << "index of 15.5: " << dList.indexOf(15.5) << endl;
}

void dlistDemo6(){
    List<int> list;
    for(int i = 0; i< 10 ; i++)
        list.add(i, i*i);
    
    cout << setw(25) << left << "Original list: ";
    list.println();
    
    //
    int& item = list.get(5);
    item = 999;
    cout << setw(25) << left << "After changing an item: ";
    list.println();
}


template <class T>
void addAtIndex(DLinkedList<T>& list, int index, T item){
    list.add(index, item);
}
template <class T>
void addData(DLinkedList<T>& list, T item){
    list.add(item);
}
template <class T>
T getData(DLinkedList<T>& list, int index){
    return list.get(index);
}
template <class T>
int indexOfData(DLinkedList<T>& list, T item){
    return list.indexOf(item);
}
template <class T>
bool containsData(DLinkedList<T>& list, T item){
    return list.contains(item);
}
template <class T>
string toString(DLinkedList<T>& list, string (*item2str)(T &)){
    return list.toString(item2str);
}
template <class T>
int size(DLinkedList<T>& list){
    return list.size();
}
template <class T>
bool isEmpty(DLinkedList<T>& list){
    return list.empty();
}
template <class T>
void clear(DLinkedList<T>& list){
    list.clear();
}
template <class T>
T removeData(DLinkedList<T>& list, T item){
    return list.remove(item);
}
template <class T>
T removeAtIndex(DLinkedList<T>& list, int index){
    return list.removeAt(index);
}
template <class T>
bool removeItem(DLinkedList<T>& list, T item, void (*removeItemData)(T)){
    return list.removeItem(item, removeItemData);
}
template <class T>
bool removeItem(DLinkedList<T>& list, T item){
    return list.removeItem(item);
}

#endif /* DLINKEDLISTDEMO_H */

