/*
    ! NGUYEN PHUC NHAN
    * Last update: 2024-09-15
    * Version 1.0
    * This file define Softmax layer
    * Softmax layer applies the softmax function to an n-dimensional input Tensor 
    *       rescaling them so that the elements of the n-dimensional output Tensor 
    *       lie in the range (0,1) and sum to 1.
*/

#ifndef SOFTMAX_H
#define SOFTMAX_H
#include "ann/Layer.h"

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

#endif /* SOFTMAX_H */

