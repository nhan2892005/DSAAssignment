/*
    ! NGUYEN PHUC NHAN
    * Last update: 2024-09-15
    * Version 1.0
    * This file implement ReLU layer
    * ReLU layer is a layer that applies the rectified linear unit activation function 
    *       element-wise to the input.
    * The ReLU function is defined as f(x) = max(0, x).
*/

#include "ann/ReLU.h"

ReLU::ReLU() {
    name = "ReLU" + to_string(++layer_idx);
}

ReLU::ReLU(const ReLU& orig) {
    name = "ReLU" + to_string(++layer_idx);
    this->mask = orig.mask;
}

ReLU::~ReLU() {
}

xt::xarray<double> ReLU::forward(xt::xarray<double> X) {
    // * ReLU function is defined as f(x) = max(0, x).
    mask = (X > 0);
    return X * mask;
}
