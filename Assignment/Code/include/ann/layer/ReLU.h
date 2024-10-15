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
#include "layer/ILayer.h"

class ReLU: public ILayer {
public:
    ReLU(string name="");
    ReLU(const ReLU& orig);
    virtual ~ReLU();
    
    xt::xarray<double> forward(xt::xarray<double> X);
    xt::xarray<double> backward(xt::xarray<double> DY);
    string get_desc();
    LayerType get_type(){ return LayerType::RELU; };
    
private:
    xt::xarray<bool> m_aMask;
};

#endif /* RELU_H */

