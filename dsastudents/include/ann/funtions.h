/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* 
 * File:   funtions.h
 * Author: ltsach
 *
 * Created on August 25, 2024, 3:28 PM
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

