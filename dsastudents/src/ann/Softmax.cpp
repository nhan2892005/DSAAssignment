/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   Softmax.cpp
 * Author: ltsach
 * 
 * Created on August 25, 2024, 2:46 PM
 */

/*
class Softmax: public Layer {
public:
    Softmax(int axis=-1);
    Softmax(const Softmax& orig);
    virtual ~Softmax();

    virtual xt::xarray<double> forward(xt::xarray<double> X);
    
private:
    int axis;
    xt::xarray<double> cached_Y;    
};
*/

#include "ann/Softmax.h"
#include "ann/funtions.h"

Softmax::Softmax(int axis): axis(axis) {
    name = "Softmax_" + to_string(++layer_idx);
}

Softmax::Softmax(const Softmax& orig) {
    /*TODO: Your code is here*/
    name = "Softmax_" + to_string(++layer_idx);
    axis = orig.axis;
    cached_Y = orig.cached_Y;
}

Softmax::~Softmax() {
}

xt::xarray<double> Softmax::forward(xt::xarray<double> X) {
    /*TODO: Your code is here*/
    cached_Y = softmax(X, axis);
    return cached_Y;
}
