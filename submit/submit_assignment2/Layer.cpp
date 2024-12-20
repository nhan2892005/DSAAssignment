/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   Layer.cpp
 * Author: ltsach
 * 
 * Created on August 25, 2024, 10:54 AM
 */

#include "layer/ILayer.h"

ILayer::ILayer() {
    this->m_trainable = false;
}

ILayer::ILayer(const ILayer& orig) {
}

ILayer::~ILayer() {
}

unsigned long long ILayer::m_unLayer_idx =0;

