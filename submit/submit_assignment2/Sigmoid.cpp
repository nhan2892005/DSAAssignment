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
    m_aCached_Y = sigmoid(X);
    return m_aCached_Y;
}
xt::xarray<double> Sigmoid::backward(xt::xarray<double> DY) {
    return DY * m_aCached_Y * (1 - m_aCached_Y);
}

string Sigmoid::get_desc(){
    string desc = fmt::format("{:<10s}, {:<15s}:",
                    "Sigmoid", this->getname());
    return desc;
}