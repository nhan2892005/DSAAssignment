/*
    ! NGUYEN PHUC NHAN
    * Last update: 2024-09-15
    * Version 1.0
    * This file implement Softmax layer
    * Softmax layer is a layer that applies the softmax activation function
    * The softmax function is defined as f(x) = exp(x) / sum(exp(x))
*/

#include "ann/Softmax.h"
#include "ann/funtions.h"

Softmax::Softmax(int axis): axis(axis) {
    name = "Softmax_" + to_string(layer_idx);
    cached_Y = xt::zeros<double>({1});
}

Softmax::Softmax(const Softmax& orig) {
    name = "Softmax_" + to_string(layer_idx);
    axis = orig.axis;
    cached_Y = orig.cached_Y;
}

Softmax::~Softmax() {
}

xt::xarray<double> Softmax::forward(xt::xarray<double> X) {
    // * Softmax function is defined as f(x) = exp(x) / sum(exp(x))
    cached_Y = softmax(X, axis);
    return cached_Y;
}
