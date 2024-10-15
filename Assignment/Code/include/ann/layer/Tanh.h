/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.h to edit this template
 */

/* 
 * File:   Tanh.h
 * Author: ltsach
 *
 * Created on September 1, 2024, 7:03 PM
 */

#ifndef TANH_H
#define TANH_H
#include "layer/ILayer.h"

class Tanh: public ILayer  {
public:
    Tanh(string name="");
    Tanh(const Tanh& orig);
    virtual ~Tanh();
    
    xt::xarray<double> forward(xt::xarray<double> X);
    xt::xarray<double> backward(xt::xarray<double> DY);
    
    string get_desc();
    LayerType get_type(){ return LayerType::TANH; };
private:
    xt::xarray<double> m_aCached_Y;
};

#endif /* TANH_H */

