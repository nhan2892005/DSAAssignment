/*
    ! NGUYEN PHUC NHAN
    * Last update: 2024-11-01
    * Version 1.0
    * This file implements the Cross Entropy class
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

double cross_entropy_stu(xt::xarray<double> Ypred, xt::xarray<double> Ygt, bool mean_reduced){
    int nsamples = Ypred.shape()[0];
    xt::xarray<double> logYpred = xt::log(Ypred + 1e-7);
    xt::xarray<double> R = -Ygt * logYpred;
    R = xt::sum(R, -1);
    
    xt::xarray<double>  sum = xt::sum(R);
    if(mean_reduced) return (sum/nsamples)[0];
    else return sum[0];
}

double CrossEntropy::forward(xt::xarray<double> X, xt::xarray<double> t){
    m_aYtarget = t;
    m_aCached_Ypred = X;
    return cross_entropy_stu(X, t, m_eReduction == REDUCE_MEAN);
}
xt::xarray<double> CrossEntropy::backward() {
    //YOUR CODE IS HERE
    // ∆y = − 1/Nnorm × t/(y + ε)
    double EPSILON = 1e-7;
    unsigned long nsamples = m_aYtarget.shape()[0];
    xt::xarray<double> YPred_plus_epsilon = m_aCached_Ypred + EPSILON;
    xt::xarray<double> TargetDivYPred = m_aYtarget/(YPred_plus_epsilon);
    xt::xarray<double> dY = -TargetDivYPred/nsamples;
    return dY;
}