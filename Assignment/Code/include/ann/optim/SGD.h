/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.h to edit this template
 */

/* 
 * File:   SGD.h
 * Author: ltsach
 *
 * Created on September 5, 2024, 5:30 PM
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

