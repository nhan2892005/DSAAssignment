/*
    ! NGUYEN PHUC NHAN
    * Last update: 2024-11-06
    * Version 1.0
    * This file implements the FCLayer class
*/

#include "loss/ILossLayer.h"

ILossLayer::ILossLayer(LossReduction reduction):
m_eReduction(reduction) {
}

ILossLayer::ILossLayer(const ILossLayer& orig):
m_eReduction(orig.m_eReduction){
}

ILossLayer::~ILossLayer() {
}

