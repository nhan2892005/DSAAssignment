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
    m_aCached_Y = tanh(X);
    return m_aCached_Y;
}
xt::xarray<double> Tanh::backward(xt::xarray<double> DY) {
    auto Y_square = m_aCached_Y * m_aCached_Y;
    auto complement_Y_square = 1 - Y_square;
    return DY * complement_Y_square;
}

string Tanh::get_desc(){
    string desc = fmt::format("{:<10s}, {:<15s}:",
                    "Tanh", this->getname());
    return desc;
}
