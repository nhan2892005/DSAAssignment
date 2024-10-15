/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.h to edit this template
 */

/* 
 * File:   Softmax.h
 * Author: ltsach
 *
 * Created on August 25, 2024, 2:46 PM
 */

#ifndef SOFTMAX_H
#define SOFTMAX_H
#include "layer/ILayer.h"

class Softmax: public ILayer {
public:
    Softmax(int axis=-1, string name="");
    Softmax(const Softmax& orig);
    virtual ~Softmax();

    virtual xt::xarray<double> forward(xt::xarray<double> X);
    virtual xt::xarray<double> backward(xt::xarray<double> DY);
    
    string get_desc();
    LayerType get_type(){ return LayerType::SOFTMAX; };
    
    //void save(string model_path);
    //void load(string model_path, string layer_name="");
private:
    int m_nAxis;
    xt::xarray<double> m_aCached_Y;    
};

#endif /* SOFTMAX_H */

