/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.cc to edit this template
 */

#include "ann/xtensor_lib.h"


string shape2str(xt::svector<unsigned long> vec){
    stringstream ss;
    ss << "(";
    for(int idx=0; idx < vec.size(); idx++){
        ss << vec[idx] << ", ";
    }
    string res = ss.str();
    if(vec.size() > 1) res = res.substr(0, res.rfind(','));
    else res = res.substr(0, res.rfind(' '));
    return res + ")";
}

int positive_index(int idx, int size){
    if(idx < 0) return idx = size + idx;
    return idx;
}

//should use einsum if it exists
xt::xarray<double> outer_stack(xt::xarray<double> X, xt::xarray<double>  Y){
    /*TODO: Your code is here*/
}
xt::xarray<double> diag_stack(xt::xarray<double> X){
    /*TODO: Your code is here*/
}
xt::xarray<double> matmul_on_stack(xt::xarray<double> X, xt::xarray<double>  Y){
    /*TODO: Your code is here*/
}


ulong_array confusion_matrix(ulong_array y_true, ulong_array y_pred){
    /*TODO: Your code is here*/
}
xt::xarray<ulong> class_count(xt::xarray<ulong> confusion){
    xt::xarray<ulong> count = xt::sum(confusion, -1);
    return count;
}

double_array calc_metrics(ulong_array y_true, ulong_array y_pred){
    /*TODO: Your code is here*/
}