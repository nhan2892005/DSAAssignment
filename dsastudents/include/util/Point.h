/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Point.h
 * Author: LTSACH
 *
 * Created on 19 August 2020, 21:03
 */

#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <iomanip>
#include <math.h>
#include <random>
#include <sstream>
using namespace std;

#define EPSILON (1E-8)

class Point{
private:
    float x, y, z;
    friend ostream &operator<<( ostream &os, const Point& point);
    
public:
    Point(float x=0.0f, float y=0.0f, float z=0.0f){
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Point(const Point& point){
        this->x = point.x;
        this->y = point.y;
        this->z = point.z;
    }
    
    void setZ(float z) {
        this->z = z;
    }

    float getZ() const {
        return z;
    }
    
    void setY(float y) {
        this->y = y;
    }

    float getY() const {
        return y;
    }

    void setX(float x) {
        this->x = x;
    }

    float getX() const {
        return x;
    }
    float radius(){
        return sqrt(x*x + y*y + z*z);
    }

    //with point object
    bool operator==(Point rhs){
        return  (abs(this->x - rhs.x) < EPSILON) &&
                (abs(this->y - rhs.y) < EPSILON) &&
                (abs(this->z - rhs.z) < EPSILON);
    }
    
    //with pointer to point
    static bool pointEQ(Point& lhs, Point& rhs){
        return  lhs == rhs;
    }
    //with pointer to point
    static bool pointEQ(Point*& lhs, Point*& rhs){
        return  *lhs == *rhs;
    }
    
    static string point2str(Point& point){
        stringstream os;
        os  << point;
        return os.str();
    }
    static string point2str(Point*& point){
        return point2str(*point);
    }

    operator float(){
        return sqrt(x*x + y*y + z*z);
    }
     
    static Point* genPoints(int size, float minValue=0, float maxValue=1, 
                            bool manualSeed=false, int seedValue=0){
        
        Point* head = new Point[size];
        
        std::default_random_engine* engine;
        if(manualSeed)
            engine = new std::default_random_engine(static_cast<long unsigned int>(seedValue));
        else
            engine = new std::default_random_engine(static_cast<long unsigned int>(time(0)));
        uniform_real_distribution<double> dist(minValue, maxValue);
        
        //
        for(int idx=0; idx < size; idx++){
            float x = dist(*engine);
            float y = dist(*engine);
            float z = dist(*engine);
            head[idx] = Point(x,y,z);
        }
        delete engine;
        return head;
    }
    static void println(Point* head, int size){
        stringstream os;
        os << "[";
        for(int idx=0; idx < size-1; idx++)
            os << head[idx] << ", ";
        if(size > 0) os << head[size-1] << "]";
        else os << "]";
        cout << os.str() << endl;
        return;
    }
   
};
ostream &operator<<( ostream &os, const Point& point){
    os << "P(" << fixed 
            << setw(6) << setprecision(2) << point.x << "," 
            << setw(6) << setprecision(2) << point.y << ","
            << setw(6) << setprecision(2) << point.z
       << ")";
    return os;
}



#endif /* POINT_H */

