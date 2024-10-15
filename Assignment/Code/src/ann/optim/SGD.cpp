/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   SGD.cpp
 * Author: ltsach
 * 
 * Created on September 5, 2024, 5:30 PM
 */

#include "optim/SGD.h"
#include "list/DLinkedList.h"
#include <string>
using namespace std;
#include "optim/SGDParamGroup.h"

SGD::SGD(double lr):IOptimizer(lr){
}

SGD::SGD(const SGD& orig) {
}

SGD::~SGD() {
}

IParamGroup* SGD::create_group(string name){
    IParamGroup* pGroup = new SGDParamGroup();
    m_pGroupMap->put(name, pGroup);
    return pGroup;
}


