/*
    ! NGUYEN PHUC NHAN
    * Last update: 2024-11-01
    * Version 1.0
    * This file defines the Ada Gradient class
*/

#ifndef ADAGRAD_H
#define ADAGRAD_H
#include "optim/IOptimizer.h"
#include "optim/IParamGroup.h"

class Adagrad: public IOptimizer{
public:
    Adagrad(double learning_rate=1e-4, double decay=0.9);
    Adagrad(const Adagrad& orig);
    virtual ~Adagrad();
    
    IParamGroup* create_group(string name);
private:
    double m_decay;
};

#endif /* ADAGRAD_H */

