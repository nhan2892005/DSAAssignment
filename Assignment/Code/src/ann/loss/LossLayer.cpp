/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   LossLayer.cpp
 * Author: ltsach
 * 
 * Created on September 2, 2024, 10:14 AM
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

