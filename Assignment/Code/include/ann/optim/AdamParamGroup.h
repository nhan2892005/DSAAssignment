/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.h to edit this template
 */

/* 
 * File:   AdamParamGroup.h
 * Author: ltsach
 *
 * Created on October 8, 2024, 1:43 PM
 */

#ifndef ADAMPARAMGROUP_H
#define ADAMPARAMGROUP_H
#include "optim/IParamGroup.h"

class AdamParamGroup: public IParamGroup {
public:
    AdamParamGroup(double beta1=0.9, double beta2=0.99);
    AdamParamGroup(const AdamParamGroup& orig);
    virtual ~AdamParamGroup();
    
    void register_param(string param_name, xt::xarray<double>* ptr_param, xt::xarray<double>* ptr_grad); //override
    void register_sample_count(unsigned long long* pCounter);
    void zero_grad();
    void step(double lr);
    
protected:
    xmap<string, xt::xarray<double>*>* m_pParams;
    xmap<string, xt::xarray<double>*>* m_pGrads;
    unsigned long long* m_pCounter;
    //
    xmap<string, xt::xarray<double>*>* m_pFirstMomment;
    xmap<string, xt::xarray<double>*>* m_pSecondMomment;

    double m_beta1, m_beta2;
    double m_step_idx; //started with 1
    double m_beta1_t, m_beta2_t; //m_beta1^t and m_beta2^t
private:
};

#endif /* ADAMPARAMGROUP_H */

