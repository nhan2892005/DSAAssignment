/*
    ! NGUYEN PHUC NHAN
    * Last update: 2024-11-01
    * Version 1.0
    * This file implements the ReLU Layer class
*/

#include "layer/ReLU.h"
#include "sformat/fmt_lib.h"
#include "ann/functions.h"

ReLU::ReLU(string name) {
    if(trim(name).size() != 0) m_sName = name;
    else m_sName = "ReLU_" + to_string(++m_unLayer_idx);
}

ReLU::ReLU(const ReLU& orig) {
    m_sName = "ReLU_" + to_string(++m_unLayer_idx);
}

ReLU::~ReLU() {
}

xt::xarray<double> ReLU::forward(xt::xarray<double> X) {
    bool one_data = false;
    if (X.shape().size() == 1) {
        X.reshape({1, X.shape()[0]});
        one_data = true;
    }
    // * ReLU function is defined as f(x) = max(0, x).
    m_aMask = (X >= 0);
    xt::xarray<double> Z = m_aMask * X;
    if(one_data){
        X.reshape({X.size()});
        Z.reshape({Z.size()});
    }
    return Z;
}
xt::xarray<double> ReLU::backward(xt::xarray<double> DY) {
    bool one_data = false;
    if (DY.shape().size() == 1) {
        DY.reshape({1, DY.shape()[0]});
        one_data = true;
    }
    xt::xarray<double> DX = DY * m_aMask;
    if(one_data){
        DY.reshape({DX.size()});
        DX.reshape({DX.size()});
    }
    return DX;
}

string ReLU::get_desc(){
    string desc = fmt::format("{:<10s}, {:<15s}:",
                    "ReLU", this->getname());
    return desc;
}