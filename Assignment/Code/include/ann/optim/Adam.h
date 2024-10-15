/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.h to edit this template
 */

/* 
 * File:   Adam.h
 * Author: ltsach
 *
 * Created on September 5, 2024, 5:31 PM
 */

#ifndef ADAM_H
#define ADAM_H
#include "optim/IParamGroup.h"
#include "optim/IOptimizer.h"

class Adam: public IOptimizer{
public:
    Adam(double lr=1e-3, double beta_1=0.9, double beta_2=0.999);
    Adam(const Adam& orig);
    virtual ~Adam();
    IParamGroup* create_group(string name);
    
private:
    double m_learning_rate;
    double m_beta_1, m_beta_2;
};

#endif /* ADAM_H */

