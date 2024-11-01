/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   CrossEntropy.cpp
 * Author: ltsach
 * 
 * Created on August 25, 2024, 2:47 PM
 */

#include "loss/CrossEntropy.h"
#include "ann/functions.h"

CrossEntropy::CrossEntropy(LossReduction reduction): ILossLayer(reduction){
    
}

CrossEntropy::CrossEntropy(const CrossEntropy& orig):
ILossLayer(orig){
}

CrossEntropy::~CrossEntropy() {
}

double CrossEntropy::forward(xt::xarray<double> X, xt::xarray<double> t){
    m_aYtarget = t;
    m_aCached_Ypred = X;
    double ce = cross_entropy(X, t, true);
    return ce;
}
xt::xarray<double> CrossEntropy::backward() {
    //YOUR CODE IS HERE
    // ∆y = − 1/Nnorm × t/(y + ε)
    unsigned long EPSILON = 1e-7;
    unsigned long nsamples = m_aYtarget.shape()[0];
    xt::xarray<double> YPred_plus_epsilon = m_aCached_Ypred + EPSILON;
    xt::xarray<double> TargetDivYPred = m_aYtarget/(YPred_plus_epsilon);
    xt::xarray<double> dY = -TargetDivYPred/nsamples;
    return dY;
}