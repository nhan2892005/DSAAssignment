/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   FCLayer.cpp
 * Author: ltsach
 * 
 * Created on August 25, 2024, 11:06 AM
 */

#include "ann/FCLayer.h"
#include "ann/funtions.h"

FCLayer::FCLayer(int in_features, int out_features, bool use_bias) {
    this->m_nIn_Features = in_features;
    this->m_nOut_Features = out_features;
    this->m_bUse_Bias = use_bias;
    name = "FC_" + to_string(++layer_idx);
    m_unSample_Counter = 0;
    
    init_weights();
}
void FCLayer::init_weights(){
   /*TODO: Your code is here*/
}

FCLayer::FCLayer(const FCLayer& orig) {
    name = "FC_" + to_string(++layer_idx);
}

FCLayer::~FCLayer() {
    /*TODO: Your code is here*/ 
}

xt::xarray<double> FCLayer::forward(xt::xarray<double> X) {
    /*TODO: Your code is here*/ 
}

static FCLayer* FCLayer::fromPretrained(string filename, bool use_bias){
    /*TODO: Your code is here*/ 
}