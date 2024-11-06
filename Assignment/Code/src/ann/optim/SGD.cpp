/*
    ! NGUYEN PHUC NHAN
    * Last update: 2024-11-01
    * Version 1.0
    * This file implements the SGD class
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


