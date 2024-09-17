#include "ann/funtions.h"
#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <limits>



xt::xarray<double> softmax(xt::xarray<double> X, int axis){
    // * Get the maximum value along the axis to avoid overflow
    auto max = xt::amax(X, {axis}, xt::keep_dims);

    // * exp(x - max) to avoid overflow
    xt::xarray<double> exps = xt::exp(X - max);

    // * sum(exp(x)) along the axis
    xt::xarray<double> sum = xt::sum(exps, {axis}, xt::keep_dims);

    // * Softmax function is defined as f(x) = exp(x) / sum(exp(x))
    return exps / sum;
}
