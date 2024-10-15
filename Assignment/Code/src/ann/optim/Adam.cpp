/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   Adam.cpp
 * Author: ltsach
 * 
 * Created on September 5, 2024, 5:31 PM
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
}

