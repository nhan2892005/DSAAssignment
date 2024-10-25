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

template <class T>
using List = DLinkedList<T>;


BaseModel::BaseModel() {
    layers = List<Layer*>(List<Layer*>::free);
}
BaseModel::BaseModel(Layer** seq, int size) {
    layers = List<Layer*>(List<Layer*>::free);
    for(int idx=0; idx < size; idx++){
        layers.add(seq[idx]);
    }
}

BaseModel::BaseModel(const BaseModel& orig) {
    layers = List<Layer*>(List<Layer*>::free);
    for(auto ptr_layer : const_cast<DLinkedList<Layer*>&>(orig.layers)){
        layers.add(ptr_layer);
    }
}

BaseModel::~BaseModel() {
}

xt::xarray<double> BaseModel::predict(xt::xarray<double> X){
    xt::xarray<double> A = X;
    for(Layer* layer: layers){
        A = layer->forward(A);
    }
    return A;
}
