/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   HeapDemo.h
 * Author: LTSACH
 *
 * Created on 22 August 2020, 18:30
 */

#ifndef HEAPDEMO_H
#define HEAPDEMO_H

#include <math.h>
#include <string>
#include <sstream>


#include "heap/Heap.h"
#include "util/Point.h"
#include "util/sampleFunc.h"

/* Min-Heap:
 Input: {50, 20, 15, 10, 8, 6, 7, 23};
 * After adding 50:
                            50
    => array: [50]
 * After adding 20: 
                            50
                           /
                         20
    => ReheapUp:
                            20
                           /
                         50  
  
    => array: [20, 50]
 * After adding 15:
                            20
                           /  \
                         50    15
    => ReheapUp:
                            15
                           /  \
                         50    20
 
    => array: [15, 50, 20]
 
 * After adding 10:
                            15
                           /  \
                         50    20
                        /
                      10
    => ReheapUp:  
                            10
                           /  \
                        15    20
                        /
                      50                 
 
    => array: [10, 15, 20, 50]
 * After adding 8:
                            10
                           /  \
                        15    20
                        / \
                      50   8   
    => ReheapUp:  
                            8
                           /  \
                         10    20
                        / \
                      50   15    
 
    => array: [8, 10, 20, 50, 15]
 
 * After adding 6:
                            8
                           /  \
                         10    20
                        / \    / 
                      50   15 6
    => ReheapUp:  
                            6
                           /  \
                         10    8
                        / \    / 
                      50   15 20  
  

    => array: [6, 10, 8, 50, 15, 20]
 * After adding 7:  
                             6
                           /   \
                         10     8
                        / \    / \
                      50   15 20  7  
    => ReheapUp: 
                             6
                           /   \
                         10     7
                        / \    / \
                      50   15 20  8   
 
    => array: [6, 10, 7, 50, 15, 20, 8]
 * After adding 23:
                             6
                           /   \
                         10     7
                        / \    / \
                      50   15 20  8   
                     /
                    23
    => ReheapUp:
                             6
                           /   \
                         10     7
                        / \    / \
                      23   15 20  8   
                     /
                    50 
   
    => array: [6, 10, 7, 23, 15, 20, 8, 50]
 */

int myIntComparator(int& lhs, int& rhs){
    if(lhs < rhs) return +1;
    else if(lhs > rhs) return -1;
    else return 0;
}
void anotherHeap(){
    Heap<int> heap;
    int array[] = {50, 20, 15, 10, 8, 6, 7, 23}; 
    for(int idx =0; idx < 8; idx++) heap.push(array[idx]);
    heap.println();
    heap.pop();
    heap.println();
    heap.push(5);
    heap.println();
    
    Heap<int> maxHeap(&myIntComparator);
    for(int idx =0; idx < 8; idx++) maxHeap.push(array[idx]);
    maxHeap.println();
    maxHeap.pop();
    maxHeap.println();
    maxHeap.push(70);
    maxHeap.println();
    
    
}
void simpleMinHeap(){
    int array[] = {50, 20, 15, 10, 8, 6, 7, 23}; 
              //    0   1   2   3  4  5  6   7
    //min heap: [6, 10, 7, 23, 15, 20, 8, 50]
    //[6,10,7,23,15,20,8,50]

    cout << "Input array: ";
    for(int idx =0; idx < 8; idx++) cout << array[idx]  << "  ";
    cout << endl;
    
    Heap<int> minHeap;
    for(int idx =0; idx < 8; idx++){
        minHeap.clear();
        minHeap.heapify(array, idx+1);
        cout << "Min heap: " << minHeap.toString() << endl;
    }
    const int& a = minHeap.peek();
    cout << "a = " << a << endl;
}

void heapDemo1(){
    int array[] = {50, 20, 15, 10, 8, 6, 7, 23}; 
    //min heap: [6, 10, 7, 23, 15, 20, 8, 50]
    //max heap: [50, 23, 15, 20, 8, 6, 7, 10]
    Heap<int> minHeap1;
    cout << "Min Heap: ";
    minHeap1.heapify(array, 8);
    cout << minHeap1.toString() << endl;
    
    Heap<int> minHeap2(minHeapComparator);
    cout << "Min Heap: ";
    minHeap2.heapify(array, 8);
    cout << minHeap2.toString() << endl;
    
    Heap<int> maxHeap(maxHeapComparator);
    cout << "Max Heap: ";
    maxHeap.heapify(array, 8);
    cout << maxHeap.toString() << endl;
}

void heapDemo2(){
    Point array[] = {
        Point(13.1, 12.4), Point(5.5, 4.5), Point(15.5, 14.5),
        Point(23.1, 12.4), Point(35.5, 14.5), Point(5.5, 34.5)
    };
    Heap<Point> minHeap1(minHeapComparator);
    cout << "Min Heap: ";
    minHeap1.heapify(array, 6);
    cout << minHeap1.toString(&point2str) << endl;
    
    Heap<Point> maxHeap(maxHeapComparator);
    cout << "Max Heap: ";
    maxHeap.heapify(array, 6);
    cout << maxHeap.toString(&point2str) << endl;
}

int myPointComparator(Point*&  lhs, Point*& rhs){
    if(lhs->radius() < rhs->radius()) return -1;
    else if(lhs->radius() > rhs->radius()) return +1;
    else return 0;
}
int myPointComparatorMAX(Point*&  lhs, Point*& rhs){
    if(lhs->radius() < rhs->radius()) return +1;
    else if(lhs->radius() > rhs->radius()) return -1;
    else return 0;
}

string myPoint2Str(Point*& point){
    stringstream os;
    os << "["   << setw(6) << setprecision(2) << point->getX() 
                << ", "
                << setw(6) << setprecision(2) << point->getY()
       << "]";
    return os.str();
}
void heapDemo3(){
    Point* array[] = {
        new Point(13.1, 12.4), new Point(5.5, 4.5), new Point(15.5, 14.5),
        new Point(23.1, 12.4), new Point(35.5, 14.5), new Point(5.5, 34.5)
    };
    Heap<Point*> heap(&myPointComparatorMAX, &Heap<Point*>::free);
    for(int idx=0; idx < 6; idx++) heap.push(array[idx]);
    heap.println(&myPoint2Str);
    
    for(Heap<Point*>::Iterator it = heap.begin(); it != heap.end(); it++){
        Point* point = *it;
        cout << point->radius() << ", ";
    }
    cout << endl;
}


#endif /* HEAPDEMO_H */

