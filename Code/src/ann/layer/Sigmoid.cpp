/*
    ! NGUYEN PHUC NHAN
    * Last update: 2024-11-01
    * Version 1.0
    * This file implements the Sigmoid Layer class
*/

#include "layer/Sigmoid.h"
#include "sformat/fmt_lib.h"
#include "ann/functions.h"

xt::xarray<double> sigmoid(xt::xarray<double> X){
    return 1 / (1 + xt::exp(-X));
}

Sigmoid::Sigmoid(string name) {
    if(trim(name).size() != 0) m_sName = name;
    else m_sName = "Sigmoid_" + to_string(++m_unLayer_idx);
}

Sigmoid::Sigmoid(const Sigmoid& orig) {
    m_sName = "Sigmoid_" + to_string(++m_unLayer_idx);
}

Sigmoid::~Sigmoid() {
}
xt::xarray<double> Sigmoid::forward(xt::xarray<double> X) {
    bool one_data = false;
    if (X.shape().size() == 1) {
        X.reshape({1, X.shape()[0]});
        one_data = true;
    }
    xt::xarray<double> Y = sigmoid(X);
    m_aCached_Y = Y;
    if(one_data){
        X.reshape({X.size()});
        Y.reshape({Y.size()});
    }
    return Y;
}
xt::xarray<double> Sigmoid::backward(xt::xarray<double> DY) {
    bool one_data = false;
    if (DY.shape().size() == 1) {
        DY.reshape({1, DY.shape()[0]});
        one_data = true;
    }
    xt::xarray<double> DX = DY * m_aCached_Y * (1 - m_aCached_Y);
    if(one_data){
        DY.reshape({DX.size()});
        DX.reshape({DX.size()});
    }
    return DX;
}

string Sigmoid::get_desc(){
    string desc = fmt::format("{:<10s}, {:<15s}:",
                    "Sigmoid", this->getname());
    return desc;
}