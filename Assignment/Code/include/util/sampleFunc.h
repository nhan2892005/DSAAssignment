/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   sampleFunc.h
 * Author: LTSACH
 *
 * Created on 10 September 2020, 20:38
 */

#ifndef SAMPLEFUNC_H
#define SAMPLEFUNC_H
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
using namespace std;

#include "util/Point.h"
#include "util/ArrayLib.h"



int minHeapComparator(int& a, int& b){
    if (a < b) return -1;
    else if(a > b) return +1;
    else return 0;
}
int maxHeapComparator(int& a, int& b){
    if (a < b) return +1;
    else if(a > b) return -1;
    else return 0;
}

int minHeapComparator(Point& a, Point& b){
    float ra = sqrt(a.getX()*a.getX() + a.getY()*a.getY());
    float rb = sqrt(b.getX()*b.getX() + b.getY()*b.getY());
    if (ra < rb) return -1;
    else if(ra > rb) return +1;
    else return 0;
}
int maxHeapComparator(Point& a, Point& b){
    float ra = sqrt(a.getX()*a.getX() + a.getY()*a.getY());
    float rb = sqrt(b.getX()*b.getX() + b.getY()*b.getY());
    if (ra < rb) return +1;
    else if(ra > rb) return -1;
    else return 0;
}
int minHeapComparator(Point*& a, Point*& b){
    float ra = sqrt(a->getX()*a->getX() + a->getY()*a->getY());
    float rb = sqrt(b->getX()*b->getX() + b->getY()*b->getY());
    if (ra < rb) return -1;
    else if(ra > rb) return +1;
    else return 0;
}
int maxHeapComparator(Point*& a, Point*& b){
    float ra = sqrt(a->getX()*a->getX() + a->getY()*a->getY());
    float rb = sqrt(b->getX()*b->getX() + b->getY()*b->getY());
    if (ra < rb) return +1;
    else if(ra > rb) return -1;
    else return 0;
}
////////////////////////////////////////////////////////////
string point2str(Point& point){
    stringstream os;
    os  << "("  << point.getX() 
                << ", "
                << point.getY()
        << ")";
    return os.str();
}
string point2str(Point*& point){
    stringstream os;
    os  << "("  << point->getX() 
                << ", "
                << point->getY()
        << ")";
    return os.str();
}

string int2str(int& value){
    stringstream os;
    os << value;
    return os.str();
}
string int2str(int*& value){
    stringstream os;
    os << *value;
    return os.str();
}

#endif /* SAMPLEFUNC_H */

