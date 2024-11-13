/*
    ! NGUYEN PHUC NHAN
    * Last update: 2024-11-01
    * Version 1.0
    * This file defines the Adam class
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

