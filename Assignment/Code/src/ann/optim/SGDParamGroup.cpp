/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   SGDParamGroup.cpp
 * Author: ltsach
 * 
 * Created on October 7, 2024, 9:45 PM
 */

#include "optim/SGDParamGroup.h"

SGDParamGroup::SGDParamGroup() {
    m_pParams = new xmap<string, xt::xarray<double>*>(&stringHash);
    m_pGrads = new xmap<string, xt::xarray<double>*>(&stringHash);
}

SGDParamGroup::SGDParamGroup(const SGDParamGroup& orig) {
}

SGDParamGroup::~SGDParamGroup() {
}

void SGDParamGroup::register_param(string param_name, xt::xarray<double>* ptr_param, xt::xarray<double>* ptr_grad){
    m_pParams->put(param_name, ptr_param);
    m_pGrads->put(param_name, ptr_grad);
}
void SGDParamGroup::register_sample_count(unsigned long long* pCounter){
    m_pCounter = pCounter;
}
void SGDParamGroup::zero_grad(){
    DLinkedList<string> keys = m_pGrads->keys();
    for(auto key: keys){
        xt::xarray<double>* pGrad = m_pGrads->get(key);
        xt::xarray<double>* pParam = m_pParams->get(key);
        *pGrad = xt::zeros<double>(pParam->shape());
    }
    //reset sample_counter
    *m_pCounter = 0;
}

void SGDParamGroup::step(double lr){
    DLinkedList<string> keys = m_pGrads->keys();
    for(auto key: keys){
        xt::xarray<double>& P = *m_pParams->get(key);
        xt::xarray<double>& grad_P = *m_pGrads->get(key);
        P = P - lr*grad_P;
    }
}
