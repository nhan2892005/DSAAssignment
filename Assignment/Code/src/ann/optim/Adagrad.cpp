/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   Adagrad.cpp
 * Author: ltsach
 * 
 * Created on September 5, 2024, 5:30 PM
 */

#include "optim/Adagrad.h"
#include "optim/AdaParamGroup.h"

Adagrad::Adagrad(double learning_rate, double decay): 
    IOptimizer(learning_rate), m_decay(decay){
}

Adagrad::Adagrad(const Adagrad& orig) {
}

Adagrad::~Adagrad() {
}

IParamGroup* Adagrad::create_group(string name){
    //YOUR CODE IS HERE
}

