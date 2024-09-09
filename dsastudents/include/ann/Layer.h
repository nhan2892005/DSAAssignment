/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.h to edit this template
 */

/* 
 * File:   Layer.h
 * Author: ltsach
 *
 * Created on August 25, 2024, 10:54 AM
 */

#ifndef LAYER_H
#define LAYER_H
#include "ann/xtensor_lib.h"
#include "ann/funtions.h"
#include <string>
using namespace std;

class Layer {
public:
    Layer();
    Layer(const Layer& orig);
    virtual ~Layer();
    
    virtual xt::xarray<double> forward(xt::xarray<double> X)=0;
    virtual string getname(){return name; }
protected:
    
    bool is_training;
    static unsigned long long layer_idx;
    string name;
private:
};


#endif /* LAYER_H */

