/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   ReLU.cpp
 * Author: ltsach
 * 
 * Created on August 25, 2024, 2:44 PM
 */

#include "ann/ReLU.h"

ReLU::ReLU() {
    name = "ReLU" + to_string(++layer_idx);
}

ReLU::ReLU(const ReLU& orig) {
    name = "ReLU" + to_string(++layer_idx);
}

ReLU::~ReLU() {
}

xt::xarray<double> ReLU::forward(xt::xarray<double> X) {
    /*TODO: Your code is here*/
}
