/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   AdagradParamGroup.cpp
 * Author: ltsach
 * 
 * Created on October 7, 2024, 9:59 PM
 */

#include "optim/AdaParamGroup.h"

AdaParamGroup::AdaParamGroup(double decay): m_decay(decay) {
    m_pParams = new xmap<string, xt::xarray<double>*>(&stringHash);
    m_pGrads = new xmap<string, xt::xarray<double>*>(&stringHash);
    m_pSquaredGrads = new xmap<string, xt::xarray<double>*>(
            &stringHash,
            0.75,
            0,
            xmap<string, xt::xarray<double>*>::freeValue);
}

AdaParamGroup::AdaParamGroup(const AdaParamGroup& orig) {
}

AdaParamGroup::~AdaParamGroup() {
}

void AdaParamGroup::register_param(string param_name, xt::xarray<double>* ptr_param, xt::xarray<double>* ptr_grad){
    m_pParams->put(param_name, ptr_param);
    m_pGrads->put(param_name, ptr_grad);
    //prepare squared-grads
    m_pSquaredGrads->put(param_name, new double_tensor);
}
void AdaParamGroup::register_sample_count(unsigned long long* pCounter){
    m_pCounter = pCounter;
}
void AdaParamGroup::zero_grad(){
    DLinkedList<string> keys = m_pGrads->keys();
    for(auto key: keys){
        xt::xarray<double>* pGrad = m_pGrads->get(key);
        xt::xarray<double>* pSquaredGrad = m_pSquaredGrads->get(key);
        xt::xarray<double>* pParam = m_pParams->get(key);
        *pGrad = xt::zeros<double>(pParam->shape());
        *pSquaredGrad = xt::zeros<double>(pParam->shape());
    }
    //reset sample_counter
    *m_pCounter = 0;
}

void AdaParamGroup::step(double lr){
    DLinkedList<string> keys = m_pGrads->keys();
    for(auto key: keys){
        xt::xarray<double>& grad_P = *m_pGrads->get(key);
        xt::xarray<double>& squared_grad = *m_pSquaredGrads->get(key);
        squared_grad = m_decay*squared_grad + (1 - m_decay)*grad_P*grad_P;
        xt::xarray<double>& P = *m_pParams->get(key);
        
        P = P - lr*grad_P/(xt::sqrt(squared_grad) + 1e-7);
    }
}