/*
    ! NGUYEN PHUC NHAN
    * Last update: 2024-11-01
    * Version 1.0
    * This file implements the Tanh Layer class
*/

#include "layer/Tanh.h"
#include "sformat/fmt_lib.h"
#include "ann/functions.h"

xt::xarray<double> tanh(xt::xarray<double> X){
    return xt::tanh(X);
}

Tanh::Tanh(string name) {
    if(trim(name).size() != 0) m_sName = name;
    else m_sName = "Tanh_" + to_string(++m_unLayer_idx);
}

Tanh::Tanh(const Tanh& orig) {
    m_sName = "Tanh_" + to_string(++m_unLayer_idx);
}

Tanh::~Tanh() {
}

xt::xarray<double> Tanh::forward(xt::xarray<double> X) {
    bool one_data = false;
    if (X.shape().size() == 1) {
        X.reshape({1, X.shape()[0]});
        one_data = true;
    }
    xt::xarray<double> Y = tanh(X);
    m_aCached_Y = Y;
    if(one_data){
        X.reshape({X.size()});
        Y.reshape({Y.size()});
    }
    return Y;
}
xt::xarray<double> Tanh::backward(xt::xarray<double> DY) {
    bool one_data = false;
    if (DY.shape().size() == 1) {
        DY.reshape({1, DY.shape()[0]});
        one_data = true;
    }
    auto Y_square = m_aCached_Y * m_aCached_Y;
    auto complement_Y_square = 1 - Y_square;
    xt::xarray<double> DX = DY * complement_Y_square;
    if(one_data){
        DY.reshape({DX.size()});
        DX.reshape({DX.size()});
    }
    return DX;
}

string Tanh::get_desc(){
    string desc = fmt::format("{:<10s}, {:<15s}:",
                    "Tanh", this->getname());
    return desc;
}
