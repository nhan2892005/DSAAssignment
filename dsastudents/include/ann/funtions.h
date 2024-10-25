/*
    ! NGUYEN PHUC NHAN
    * Last update: 2024-09-15
    * Version 1.0
    * This file defines the funtions (softmax) for the neural network
*/

#ifndef FUNTIONS_H
#define FUNTIONS_H
#include <string>
#include <sstream>
using namespace std;
#include <memory>
#include <stdexcept>
#include "ann/xtensor_lib.h"

xt::xarray<double> softmax(xt::xarray<double> X, int axis=-1);

#endif /* FUNTIONS_H */

