/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.cc to edit this template
 */

#include "tensor/xtensor_lib.h"


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
    xt::xarray<double> S = xt::zeros<double>({X.shape()[0], X.shape()[1], Y.shape()[1]});
    int nrows = X.shape()[0];

    for(int r=0; r < nrows; r++){
        xt::xarray<double> x = xt::row(X, r);
        xt::xarray<double> y = xt::row(Y, r);
        xt::xarray<double> U = xt::linalg::outer(x, y);
        xt::view(S, r) = U;
    }
    return S;
}
xt::xarray<double> diag_stack(xt::xarray<double> X){
    xt::xarray<double> DS = xt::zeros<double>({X.shape()[0], X.shape()[1], X.shape()[1]});
    int nrows = X.shape()[0];

    for(int r=0; r < nrows; r++){
        xt::xarray<double> x = xt::row(X, r);
        xt::xarray<double> D = xt::diag(x);
        xt::view(DS, r) = D;
    }
    return DS;
}
xt::xarray<double> matmul_on_stack(xt::xarray<double> X, xt::xarray<double>  Y){
    xt::xarray<double> S = xt::zeros<double>({X.shape()[0], X.shape()[1]});
    int nrows = X.shape()[0];
    
    for(int r=0; r < nrows; r++){
        xt::xarray<double> M = xt::view(X, r); //a matrix
        xt::xarray<double> v = xt::view(Y, r); //a vector
        xt::view(S, r) = xt::linalg::dot(M, v);
    }
    return S;
}

