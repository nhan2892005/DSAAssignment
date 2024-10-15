/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.h to edit this template
 */

/* 
 * File:   CrossEntropy.h
 * Author: ltsach
 *
 * Created on August 25, 2024, 2:47 PM
 */

#ifndef CROSSENTROPY_H
#define CROSSENTROPY_H
#include "loss/ILossLayer.h"

class CrossEntropy: public ILossLayer {
public:
    //CrossEntropy(int nclasses=2, LossReduction reduction=REDUCE_MEAN);
    CrossEntropy(LossReduction reduction=REDUCE_MEAN);
    CrossEntropy(const CrossEntropy& orig);
    virtual ~CrossEntropy();
    
    virtual double forward(xt::xarray<double> X, xt::xarray<double> t);
    virtual xt::xarray<double> backward();
    
private:
    xt::xarray<double> m_aYtarget;
    xt::xarray<double> m_aCached_Ypred;  
    //int m_nClasses;
};

#endif /* CROSSENTROPY_H */

