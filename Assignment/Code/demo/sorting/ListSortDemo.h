/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ListSort.h
 * Author: LTSACH
 *
 * Created on 12 September 2020, 09:56
 */

#ifndef LISTSORT_H
#define LISTSORT_H

#include "sorting/DLinkedListSE.h"
#include "sorting/SLinkedListSE.h"
#include "sorting/XArrayListSE.h"
#include "util/Point.h"

int sortIntComparator(int& lhs, int& rhs){
    if(lhs < rhs) return +1;
    else if(lhs > rhs) return -1;
    else return 0;
}
void listSortDemo1(){
    int items[] = {18, 5, 12, 7, 89, 1, 3, 24};
    DLinkedListSE<int> list;
    for(int i=0; i < 8; i++) list.add(items[i]);
    list.println();
    list.sort();
    list.println();
    list.sort(&sortIntComparator);
    list.println();
}

int sortPointComparator(Point*& lhs, Point*& rhs){
    if(lhs->getX() < rhs->getX()) return -1;
    else if(lhs->getX() > rhs->getX()) return +1;
    else return 0;
}

int sortPointComparatorDESC_Y(Point*& lhs, Point*& rhs){
    if(lhs->getY() < rhs->getY()) return +1;
    else if(lhs->getY() > rhs->getY()) return -1;
    else return 0;
}

void listSortDemo2(){
    int size = 20;
    Point* items = Point::genPoints(size, -10, 10);
    DLinkedListSE<Point*> list;
    for(int i=0; i < size; i++) list.add(&items[i]);
    
    list.println(&Point::point2str);
    
    cout << "Ascending:" << endl;
    list.sort(&sortPointComparator);
    list.println(&Point::point2str);
    
    cout << "Descending:" << endl;
    list.sort(&sortPointComparatorDESC_Y);
    list.println(&Point::point2str);
 
    //
    delete []items;
}

void listSortDemo3(){
    int size = 20;
    Point* items = Point::genPoints(size, -10, 10);
    XArrayListSE<Point*> list;
    for(int i=0; i < size; i++) list.add(&items[i]);
    
    list.println(&Point::point2str);
    
    cout << "Ascending:" << endl;
    list.sort(&sortPointComparator);
    list.println(&Point::point2str);
    
    cout << "Descending:" << endl;
    list.sort(&sortPointComparatorDESC_Y);
    list.println(&Point::point2str);
 
    //
    delete []items;
}

void listSortDemo4(){
    int size = 20;
    Point* items = Point::genPoints(size, -10, 10);
    SLinkedListSE<Point*> list;
    for(int i=0; i < size; i++) list.add(&items[i]);
    
    list.println(&Point::point2str);
    
    cout << "Ascending:" << endl;
    list.sort(&sortPointComparator);
    list.println(&Point::point2str);
    
    cout << "Descending:" << endl;
    list.sort(&sortPointComparatorDESC_Y);
    list.println(&Point::point2str);
 
    //
    delete []items;
}

#endif /* LISTSORT_H */

