/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   Model.cpp
 * Author: ltsach
 * 
 * Created on September 1, 2024, 5:09 PM
 */

#include "ann/BaseModel.h"
#include "ann/xtensor_lib.h"


BaseModel::BaseModel() {
    /*TODO: Your code is here*/ 
}
BaseModel::BaseModel(Layer** seq, int size) {
    /*TODO: Your code is here*/ 
}

BaseModel::BaseModel(const BaseModel& orig) {
    /*TODO: Your code is here*/ 
}

BaseModel::~BaseModel() {
    for(auto ptr_layer: layers) delete ptr_layer;
}

xt::xarray<double> BaseModel::predict(xt::xarray<double> X){
    /*TODO: Your code is here*/ 
}
