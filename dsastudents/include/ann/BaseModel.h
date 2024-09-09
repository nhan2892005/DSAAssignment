/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.h to edit this template
 */

/* 
 * File:   Model.h
 * Author: ltsach
 *
 * Created on September 1, 2024, 5:09 PM
 */

#ifndef MODEL_H
#define MODEL_H
#include "ann/xtensor_lib.h"

#include "list/DLinkedList.h"
#include "ann/Layer.h"
#include "ann/dataloader.h"

class BaseModel {
public:
    BaseModel();
    BaseModel(Layer** seq, int size);
    BaseModel(const BaseModel& orig);
    virtual ~BaseModel();
    
    virtual xt::xarray<double> predict(xt::xarray<double> X);
protected:
    DLinkedList<Layer*> layers;
};

#endif /* MODEL_H */

