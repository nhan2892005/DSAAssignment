/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.h to edit this template
 */

/* 
 * File:   AdagradParamGroup.h
 * Author: ltsach
 *
 * Created on October 7, 2024, 9:59 PM
 */

#ifndef ADAGRADPARAMGROUP_H
#define ADAGRADPARAMGROUP_H
#include "optim/IParamGroup.h"

class AdaParamGroup: public IParamGroup{
public:
    AdaParamGroup(double decay=0.9);
    AdaParamGroup(const AdaParamGroup& orig);
    virtual ~AdaParamGroup();
    
    void register_param(string param_name, xt::xarray<double>* ptr_param, xt::xarray<double>* ptr_grad); //override
    void register_sample_count(unsigned long long* pCounter);
    void zero_grad();
    void step(double lr);

protected:
    xmap<string, xt::xarray<double>*>* m_pParams;
    xmap<string, xt::xarray<double>*>* m_pGrads;
    xmap<string, xt::xarray<double>*>* m_pSquaredGrads;
    unsigned long long* m_pCounter;
    double m_decay;
private:
};

#endif /* ADAGRADPARAMGROUP_H */

