/*
    ! NGUYEN PHUC NHAN
    * Last update: 2024-09-15
    * Version 1.0
    * This file define ReLU layer
    * ReLU layer is a layer that applies the rectified linear unit activation function 
    *       element-wise to the input.
    * The ReLU function is defined as f(x) = max(0, x).
*/

#ifndef RELU_H
#define RELU_H
#include "ann/Layer.h"

class ReLU: public Layer {
public:
    ReLU();
    ReLU(const ReLU& orig);
    virtual ~ReLU();
    
    xt::xarray<double> forward(xt::xarray<double> X);
private:
    xt::xarray<bool> mask;
};

#endif /* RELU_H */

