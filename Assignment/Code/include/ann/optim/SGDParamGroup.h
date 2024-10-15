/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.h to edit this template
 */

/* 
 * File:   SGDParamGroup.h
 * Author: ltsach
 *
 * Created on October 7, 2024, 9:45 PM
 */

#ifndef SGDPARAMGROUP_H
#define SGDPARAMGROUP_H
#include "optim/IParamGroup.h"

class SGDParamGroup: public IParamGroup {
public:
    SGDParamGroup();
    SGDParamGroup(const SGDParamGroup& orig);
    virtual ~SGDParamGroup();

    void register_param(string param_name, xt::xarray<double>* ptr_param, xt::xarray<double>* ptr_grad); //override
    void register_sample_count(unsigned long long* pCounter);
    void zero_grad();
    void step(double lr);
    
protected:
    xmap<string, xt::xarray<double>*>* m_pParams;
    xmap<string, xt::xarray<double>*>* m_pGrads;
    unsigned long long* m_pCounter;
    
private:
};

#endif /* SGDPARAMGROUP_H */

