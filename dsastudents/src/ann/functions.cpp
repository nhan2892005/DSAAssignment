#include "ann/funtions.h"
#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <limits>



xt::xarray<double> softmax(xt::xarray<double> X, int axis){
    /*TODO: Your code is here*/
    auto max = xt::amax(X, {axis}, xt::keep_dims(true));
    xt::xarray<double> exps = xt::exp(X - max);
    xt::xarray<double> sum = xt::sum(exps, {axis}, xt::keep_dims(true));
    return exps / sum;
}
