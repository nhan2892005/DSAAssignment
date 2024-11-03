/*
    ! NGUYEN PHUC NHAN
    * Last update: 2024-11-01
    * Version 1.0
    * This file implements the Adam class
*/

#include "optim/Adam.h"
#include "optim/AdamParamGroup.h"

Adam::Adam(double lr, double beta_1, double beta_2):
    IOptimizer(lr), m_beta_1(beta_1), m_beta_2(beta_2) {
}

Adam::Adam(const Adam& orig):
    IOptimizer(orig.m_learning_rate), m_beta_1(orig.m_beta_1), m_beta_2(orig.m_beta_2){
}

Adam::~Adam() {
}

IParamGroup* Adam::create_group(string name){
    //YOUR CODE IS HERE
    IParamGroup* pGroup = new AdamParamGroup(m_beta_1, m_beta_2);
    m_pGroupMap->put(name, pGroup);
    return pGroup;
}

