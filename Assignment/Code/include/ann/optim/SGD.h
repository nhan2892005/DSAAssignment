/*
    ! NGUYEN PHUC NHAN
    * Last update: 2024-11-01
    * Version 1.0
    * This file defines the SGD class
*/

#ifndef SGD_H
#define SGD_H
#include "optim/IOptimizer.h"

class SGD: public IOptimizer{
public:
    SGD(double lr=1e-4);
    SGD(const SGD& orig);
    virtual ~SGD();
    
    IParamGroup* create_group(string name);
protected:

};

#endif /* SGD_H */

