/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.h to edit this template
 */

/* 
 * File:   LossLayer.h
 * Author: ltsach
 *
 * Created on September 2, 2024, 10:14 AM
 */

#ifndef LOSSLAYER_H
#define LOSSLAYER_H
#include "tensor/xtensor_lib.h"

enum LossReduction{
    REDUCE_NONE = 0,
    REDUCE_SUM,
    REDUCE_MEAN
};

class ILossLayer {
public:
    ILossLayer(LossReduction reduction=REDUCE_MEAN);
    ILossLayer(const ILossLayer& orig);
    virtual ~ILossLayer();
    
    virtual double forward(xt::xarray<double> X, xt::xarray<double> t)=0;
    virtual xt::xarray<double> backward()=0;
protected:
    LossReduction m_eReduction;
};

#endif /* LOSSLAYER_H */

