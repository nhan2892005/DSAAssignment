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

/*
class BaseModel {
public:
    BaseModel();
    BaseModel(Layer** seq, int size);
    BaseModel(const BaseModel& orig);
    virtual ~BaseModel();
    
    virtual xt::xarray<double> predict(xt::xarray<double> X);
protected:
    DLinkedList<Layer*> layers;
};
*/
BaseModel::BaseModel() {
    /*TODO: Your code is here*/ 
    layers = DLinkedList<Layer*>();
}
BaseModel::BaseModel(Layer** seq, int size) {
    /*TODO: Your code is here*/ 
    layers = DLinkedList<Layer*>();
    for(int idx=0; idx < size; idx++){
        layers.add(seq[idx]);
    }
}

BaseModel::BaseModel(const BaseModel& orig) {
    /*TODO: Your code is here*/ 
    layers = DLinkedList<Layer*>();
    for(int idx=0; idx < orig.layers.size(); idx++){
        layers.add(orig.layers.get(idx));
    }
}

BaseModel::~BaseModel() {
    for(auto ptr_layer: layers) delete ptr_layer;
}

xt::xarray<double> BaseModel::predict(xt::xarray<double> X){
    /*TODO: Your code is here*/ 
    xt::xarray<double> Y = X;
    for(auto ptr_layer: layers){
        Y = ptr_layer->forward(Y);
    }
    return Y;
}
