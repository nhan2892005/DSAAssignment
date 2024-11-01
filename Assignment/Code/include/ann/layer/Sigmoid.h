/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.h to edit this template
 */

/* 
 * File:   Sigmoid.h
 * Author: ltsach
 *
 * Created on August 25, 2024, 2:45 PM
 */

#ifndef SIGMOID_H
#define SIGMOID_H
#include "layer/ILayer.h"

class Sigmoid: public ILayer {
public:
    Sigmoid(string name="");
    Sigmoid(const Sigmoid& orig);
    virtual ~Sigmoid();
    
    xt::xarray<double> forward(xt::xarray<double> X);
    xt::xarray<double> backward(xt::xarray<double> DY);
    
    string get_desc();
    LayerType get_type(){ return LayerType::SIGMOID; };
private:
    xt::xarray<double> m_aCached_Y;

};

#endif /* SIGMOID_H */

