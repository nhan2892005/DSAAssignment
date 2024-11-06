/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.h to edit this template
 */

/* 
 * File:   Optimizer.h
 * Author: ltsach
 *
 * Created on September 2, 2024, 10:51 AM
 */

#ifndef OPTIMIZER_H
#define OPTIMIZER_H
#include "tensor/xtensor_lib.h"
#include "ann/functions.h"
#include "optim/IParamGroup.h"
#include "dsaheader.h"


class IOptimizer {
public:
    IOptimizer(double learning_rate=1e-4);
    IOptimizer(const IOptimizer& orig);
    virtual ~IOptimizer();

    virtual int num_group(){return m_pGroupMap->size(); }
    virtual void zero_grad();
    virtual void step();
    virtual IParamGroup* create_group(string name)=0;

protected:
    double m_fLearningRate;
    
    xmap<string, IParamGroup*>* m_pGroupMap;
};

#endif /* OPTIMIZER_H */

