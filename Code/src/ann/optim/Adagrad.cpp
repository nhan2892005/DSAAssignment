/*
    ! NGUYEN PHUC NHAN
    * Last update: 2024-11-01
    * Version 1.0
    * This file implements the Ada Gradient class
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
    IParamGroup* pGroup = new AdaParamGroup(m_decay);
    m_pGroupMap->put(name, pGroup);
    return pGroup;
}

