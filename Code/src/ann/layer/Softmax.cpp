/*
    ! NGUYEN PHUC NHAN
    * Last update: 2024-11-01
    * Version 1.0
    * This file implements the Softmax Layer class
*/

#include "layer/Softmax.h"
#include "ann/functions.h"
#include "sformat/fmt_lib.h"
#include <filesystem> //require C++17
namespace fs = std::filesystem;

Softmax::Softmax(int axis, string name): m_nAxis(axis) {
    if(trim(name).size() != 0) m_sName = name;
    else m_sName = "Softmax_" + to_string(++m_unLayer_idx);
}

Softmax::Softmax(const Softmax& orig) {
}

Softmax::~Softmax() {
}

xt::xarray<double> Softmax::forward(xt::xarray<double> X) {
    bool one_data = false;
    if (X.shape().size() == 1) {
        X.reshape({1, X.shape()[0]});
        one_data = true;
    }
    // * Softmax function is defined as f(x) = exp(x) / sum(exp(x))
    xt::xarray<double> Y = softmax(X, m_nAxis);
    m_aCached_Y = Y;
    if(one_data){
        X.reshape({X.size()});
        Y.reshape({Y.size()});
    }
    return Y;
}
xt::xarray<double> Softmax::backward(xt::xarray<double> DY) {
    bool one_data = false;
    if (DY.shape().size() == 1) {
        DY.reshape({1, DY.shape()[0]});
        one_data = true;
    }
    auto diag_Y = diag_stack(m_aCached_Y);
    auto outer_Y = outer_stack(m_aCached_Y, m_aCached_Y);
    xt::xarray<double> DX = matmul_on_stack(diag_Y - outer_Y, DY);
    if(one_data){
        DY.reshape({DX.size()});
        DX.reshape({DX.size()});
    }
    return DX;
}

string Softmax::get_desc(){
    string desc = fmt::format("{:<10s}, {:<15s}: {:4d}",
                    "Softmax", this->getname(), m_nAxis);
    return desc;
}
