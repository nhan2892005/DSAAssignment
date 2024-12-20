/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   QueueDemo.h
 * Author: LTSACH
 *
 * Created on 10 September 2020, 16:51
 */

#ifndef QUEUEDEMO_H
#define QUEUEDEMO_H

#include <iostream>
#include <iomanip>
#include "util/Point.h"
#include "stacknqueue/Queue.h"
using namespace std;

void queueDemo1(){
    Queue<int> queue;
    for(int i=0; i < 5; i++){
        queue.push(i);
    }
    queue.println();
    
    for(int i=-3; i < 10; i++){
        cout <<"value " << i <<  " : " << (queue.contains(i) ? "in queue" : "not in queue") << endl;
    }
}

void queueDemo2(){
    Queue<Point> queue;
    queue.push(Point(23.2f, 25.4f));
    queue.push(Point(24.6f, 23.1f));  
    
    queue.println();
    
    cout << queue.contains(Point(23.2f, 25.4f)) << endl;
    cout << queue.contains(Point(24.6f, 23.1f)) << endl;
    cout << queue.contains(Point(123.2f, 25.4f)) << endl;
    cout << queue.contains(Point(124.6f, 23.1f)) << endl;
}

string qPointPtr2Str(Point*& point){
    stringstream os;
    os << "("   << point->getX() << ", "
                << point->getY()
       << ")";
    return os.str();
}
void queueDemo3(){
    Queue<Point*> queue(&DLinkedList<Point*>::free, &Point::pointEQ);
    
    Point* p1 = new Point(23.2f, 25.4f);
    Point* p2 = new Point(24.6f, 23.1f);
    Point* p3 = new Point(123.2f, 25.4f);
    Point* p4 = new Point(124.6f, 23.1f);
    queue.push(p1);
    queue.push(p2);
    queue.push(p3);
    queue.push(p4);
    
    queue.println(&qPointPtr2Str);
    
    cout << queue.contains(new Point(23.2f, 25.4f)) << endl;
    cout << queue.contains(new Point(24.6f, 23.1f)) << endl;
    cout << queue.contains(new Point(123.2f, 25.4f)) << endl;
    cout << queue.contains(new Point(124.6f, 23.1f)) << endl;
    
    delete p1; delete p2; delete p3; delete p4;
}

#endif /* QUEUEDEMO_H */

