/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.h to edit this template
 */

/* 
 * File:   ReLU.h
 * Author: ltsach
 *
 * Created on August 25, 2024, 2:44 PM
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

